#include "DisplayManager.h"
#include "error_codes.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <mutex>
#include <list>

#include <iostream>

namespace ORC_NAMESPACE
{

        static std::mutex _mutex;
        static uint32 _ref_count = 0;

        // Lists are used here because I didn't want to deal with iterators being invalidated
        static std::list<std::pair<uint32, void*>> _windows;

        static std::list<std::pair<uint32, DisplayManager::MouseButtonCallback>> _mouse_button;
        static std::list<std::pair<uint32, DisplayManager::KeyboardCallback>> _keyboard;
        static std::list<std::pair<uint32, DisplayManager::MouseMovementCallback>> _mouse_movement;
        static std::list<std::pair<uint32, DisplayManager::MouseWheelCallback>> _mouse_wheel;

        void GLInit()
        {
                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);
                glCullFace(GL_BACK);
        }

        //////////////////////////////////////////////////////////////////////////

        static void WindowEventHandler(SDL_WindowEvent& e)
        {
                switch (e.event)
                {
                case SDL_WINDOWEVENT_CLOSE:
                        DisplayManager::DestroyWindow(e.windowID);
                        break;
                }
        }

        static void KeyboardEventHandler(SDL_KeyboardEvent& e)
        {
                // if (e.repeat) return;

                for (auto& pair : _keyboard)
                        if (pair.first == e.windowID) pair.second(e.keysym.sym, e.state != 0);
        }

        static void MouseButtonEventHandler(SDL_MouseButtonEvent& e)
        {
                for (auto& pair : _mouse_button)
                        if (pair.first == e.windowID) pair.second(e.button, e.state != 0);
        }

        static void MouseMovementEventHandler(SDL_MouseMotionEvent& e)
        {
                for (auto& pair : _mouse_movement)
                        if (pair.first == e.windowID) pair.second(e.xrel, e.yrel);
        }

        static void MouseWheelEventHandler(SDL_MouseWheelEvent& e)
        {
                for (auto& pair : _mouse_wheel)
                        if (pair.first == e.windowID) pair.second(e.y);
        }

        void DisplayManager::EnterMessageLoop()
        {
                bool quit = false;
                SDL_Event e;
                while (!ExitRequested)
                {
                        SDL_WaitEvent(&e);
                        switch (e.type)
                        {
                        case SDL_QUIT:
                                quit = true;
                                break;
                        case SDL_WINDOWEVENT:
                                WindowEventHandler(e.window);
                                break;
                        case SDL_MOUSEBUTTONDOWN:
                        case SDL_MOUSEBUTTONUP:
                                MouseButtonEventHandler(e.button);
                                break;
                        case SDL_MOUSEMOTION:
                                MouseMovementEventHandler(e.motion);
                                break;
                        case SDL_MOUSEWHEEL:
                                MouseWheelEventHandler(e.wheel);
                                break;
                        case SDL_KEYDOWN:
                        case SDL_KEYUP:
                                KeyboardEventHandler(e.key);
                                break;
                        }
                }
        }

        uint32 DisplayManager::CreateWindow(const char* title, uint32 width, uint32 height)
        {
                _mutex.lock();
                DEFER(_mutex.unlock());

                if (_ref_count == 0) // first instance
                {
                        if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
                                throw Error::SDL_VIDEO_INITIALIZATION;

                        // Requires OpenGL 2.0 or better
                        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
                        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

                        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
                        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
                        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

                        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
                        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

                        // Request full control of inputs
                        SDL_SetRelativeMouseMode(SDL_TRUE);

                }

                SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
                if (!window) throw Error::SDL_WINDOW_CREATION;

                _ref_count++;

                uint32 windowID = SDL_GetWindowID(window);
                _windows.emplace_back(windowID, nullptr);
                return windowID;
        }

        void DisplayManager::DestroyWindow(uint32 windowID)
        {
                _mutex.lock();
                DEFER(_mutex.unlock());

                for (auto& pair : _windows)
                {
                        if (pair.first == windowID)
                        {
                                void* context = pair.second;
                                if (context) SDL_GL_DeleteContext(context);

                                SDL_Window* window = SDL_GetWindowFromID(windowID);
                                if (window) SDL_DestroyWindow(window);

                                for (auto itr = _keyboard.begin(); itr != _keyboard.end();)
                                {
                                        if (itr->first == windowID) _keyboard.erase(itr++);
                                        else itr++;
                                }
                                for (auto itr = _mouse_button.begin(); itr != _mouse_button.end();)
                                {
                                        if (itr->first == windowID) _mouse_button.erase(itr++);
                                        else itr++;
                                }
                                for (auto itr = _mouse_movement.begin(); itr != _mouse_movement.end();)
                                {
                                        if (itr->first == windowID) _mouse_movement.erase(itr++);
                                        else itr++;
                                }

                                _ref_count--;
                                break;
                        }
                }

                if (_ref_count == 0)
                {
                        SDL_QuitSubSystem(SDL_INIT_VIDEO);
                        _mouse_button.clear();
                        _keyboard.clear();
                        _mouse_movement.clear();
                        _mouse_wheel.clear();
                        ExitRequested = true;
                }
        }

        void DisplayManager::SetVisible(uint32 windowID, bool visible)
        {
                SDL_Window* window = SDL_GetWindowFromID(windowID);
                if (visible) SDL_ShowWindow(window);
                else SDL_HideWindow(window);
        }

        void DisplayManager::Present(uint32 windowID)
        {
                SDL_Window* window = SDL_GetWindowFromID(windowID);
                SDL_GL_SwapWindow(window);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void DisplayManager::CreateOpenGLContext(uint32 windowID)
        {
                SDL_Window* window = SDL_GetWindowFromID(windowID);
                void* context = SDL_GL_CreateContext(window);
                if (!context)
                {
                        SDL_DestroyWindow(window);
                        throw Error::OPENGL_INITIALIZATION;
                }

                glewExperimental = GL_TRUE;
                if (glewInit() != GLEW_OK)
                {
                        SDL_GL_DeleteContext(context);
                        SDL_DestroyWindow(window);
                        throw Error::GLEW_INITIALIZATION;
                }
                glGetError(); // HACK: there is a bug in GLEW, ignore the error generated by its initialization

                GLInit();

                int32 width, height;
                SDL_GetWindowSize(window, &width, &height);
                glViewport(0,0,width, height);

                for (auto& _window : _windows)
                {
                        if (_window.first == windowID)
                        {
                                _window.second = context;
                                break;
                        }
                }
        }

        void DisplayManager::AddMouseButtonHandler(uint32 windowID, MouseButtonCallback callback)
        {
                _mouse_button.emplace_back(windowID, callback);
        }

        void DisplayManager::AddKeyboardHandler(uint32 windowID, KeyboardCallback callback)
        {
                _keyboard.emplace_back(windowID, callback);
        }

        void DisplayManager::AddMouseMovementHandler(uint32 windowID, MouseMovementCallback callback)
        {
                _mouse_movement.emplace_back(windowID, callback);
        }

        void DisplayManager::AddMouseWheelHandler(uint32 windowID, MouseWheelCallback callback)
        {
                _mouse_wheel.emplace_back(windowID, callback);
        }

        bool DisplayManager::ExitRequested = false;

};