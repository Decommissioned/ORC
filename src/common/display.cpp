#include "display.h"
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <mutex>

static std::mutex M;

namespace ORC_NAMESPACE
{

        Display::Display(const char* title, int32 width, int32 height, bool create_context /*= true*/)
        {
                M.lock();
                DEFER(M.unlock());

                if (_ref_count == 0) // first instance
                {
                        _ref_count++;

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

                _wnd = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

                if (!_wnd) throw Error::SDL_WINDOW_CREATION;
                _wndID = SDL_GetWindowID(static_cast<SDL_Window*>(_wnd));
                
                if (create_context)
                {
                        Error e = CreateOpenGLContext();
                        if (e != Error::OK) throw e;
                }

                SetVisible(true);

        }

        Display::~Display()
        {
                _ref_count--;

                if (_glContext)
                        SDL_GL_DeleteContext(_glContext);

                if (_wnd)
                {
                        SDL_Window* wnd = static_cast<SDL_Window*>(_wnd);
                        SDL_DestroyWindow(wnd);
                }

                if (_ref_count == 0)
                        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        }

        void Display::Present() const
        {
                SDL_Window* wnd = static_cast<SDL_Window*>(_wnd);
                SDL_GL_SwapWindow(wnd);
        }

        void Display::SetVisible(bool visible)
        {
                SDL_Window* wnd = static_cast<SDL_Window*>(_wnd);
                if (visible) SDL_ShowWindow(wnd);
                else SDL_HideWindow(wnd);
        }

        Error Display::CreateOpenGLContext()
        {
                SDL_Window* wnd = static_cast<SDL_Window*>(_wnd);
                
                _glContext = SDL_GL_CreateContext(wnd);
                if (!_glContext)
                        return Error::OPENGL_INITIALIZATION;

                if (glewInit() != GLEW_OK)
                        return Error::GLEW_INITIALIZATION;

                return Error::OK;
        }

        uint32 Display::ID() const
        {
                return _wndID;
        }

        uint32 Display::_ref_count = 0;

};