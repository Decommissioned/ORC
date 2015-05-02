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

        static std::list<std::pair<uint32, void*>> _windows;

        static std::list<std::pair<uint32, DisplayManager::MouseCallback>> _mouse;
        static std::list<std::pair<uint32, DisplayManager::KeyboardCallback>> _keyboard;
        static std::list<std::pair<uint32, DisplayManager::MouseMovementCallback>> _mouse_movement;

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
                for (auto& pair : _keyboard)
                {
                        if (pair.first == 0 || pair.first == e.windowID)
                                pair.second(e.keysym.sym, e.state);
                }
        }

        static void MouseEventHandler(SDL_MouseButtonEvent& e)
        {
                for (auto& pair : _mouse)
                {
                        if (pair.first == 0 || pair.first == e.windowID)
                                pair.second(e.button, e.state);
                }
        }

        static void MouseMovementEventHandler(SDL_MouseMotionEvent& e)
        {
                for (auto& pair : _mouse_movement)
                {
                        if (pair.first == 0 || pair.first == e.windowID)
                                pair.second(e.xrel, e.yrel);
                }
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
                                MouseEventHandler(e.button);
                                break;
                        case SDL_MOUSEMOTION:
                                MouseMovementEventHandler(e.motion);
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

                        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
                        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

                        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
                        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
                        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);

                        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
                        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

                }

                SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
                if (!window) throw Error::SDL_WINDOW_CREATION;

                _ref_count++;

                uint32 windowID = SDL_GetWindowID(window);
                _windows.emplace_back(std::make_pair(windowID, nullptr));
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
                                for (auto itr = _mouse.begin(); itr != _mouse.end(); ++itr)
                                {
                                        if (itr->first == windowID) _mouse.erase(itr++);
                                        else itr++;
                                }
                                for (auto itr = _mouse_movement.begin(); itr != _mouse_movement.end(); ++itr)
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
                        _mouse.clear();
                        _keyboard.clear();
                        _mouse_movement.clear();
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

                for (auto& _window : _windows)
                {
                        if (_window.first == windowID)
                        {
                                _window.second = context;
                                break;
                        }
                }
        }

        void DisplayManager::AddMouseHandler(uint32 windowID, MouseCallback& callback)
        {
                _mouse.emplace_back(std::make_pair(windowID, callback));
        }

        void DisplayManager::AddKeyboardHandler(uint32 windowID, KeyboardCallback& callback)
        {
                _keyboard.emplace_back(std::make_pair(windowID, callback));
        }

        void DisplayManager::AddMouseMovementHandler(uint32 windowID, MouseMovementCallback& callback)
        {
                _mouse_movement.emplace_back(std::make_pair(windowID, callback));
        }

        bool DisplayManager::ExitRequested = false;

};