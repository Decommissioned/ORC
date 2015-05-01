#include "DisplayManager.h"
#include "error_codes.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <mutex>
#include <vector>

#include <thread>

namespace ORC_NAMESPACE
{

        static std::mutex _mutex;
        static uint32 _ref_count = 0;

        static std::vector<std::pair<uint32, void*>> _windows;

        void DisplayManager::EnterMessageLoop()
        {
                bool quit = false;
                SDL_Event e;
                while (!_exit_requested)
                {
                        SDL_WaitEvent(&e);
                        switch (e.type)
                        {
                        case SDL_QUIT: quit = true;
                                break;
                        case SDL_WINDOWEVENT:
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
                        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

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

                                _ref_count--;
                                break;
                        }
                }

                if (_ref_count == 0)
                        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        }

        bool DisplayManager::ExitRequested()
        {
                return _exit_requested;
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

                if (glewInit() != GLEW_OK)
                {
                        SDL_GL_DeleteContext(context);
                        SDL_DestroyWindow(window);
                        throw Error::GLEW_INITIALIZATION;
                }

                for (auto& _window : _windows)
                {
                        if (_window.first == windowID)
                        {
                                _window.second = context;
                                break;
                        }
                }
        }

        bool DisplayManager::_exit_requested = false;

};