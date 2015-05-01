#include <SDL2/SDL.h>
#include <GL/glew.h>

bool run = true;

void MakeWindow()
{
        SDL_Window* wnd;
        SDL_GLContext context;

        SDL_Init(SDL_INIT_VIDEO);
                
        wnd = SDL_CreateWindow("TEST MESSAGE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

        context = SDL_GL_CreateContext(wnd);
        
        auto ret = glewInit();

        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

        SDL_ShowWindow(wnd);

        while (run)
        {
                SDL_Event e;
                if (SDL_PollEvent(&e))
                {
                        switch (e.type)
                        {
                        case SDL_QUIT: run = false;
                        }
                }
                else
                {
                        glClear(GL_COLOR_BUFFER_BIT);
                        SDL_GL_SwapWindow(wnd);
                }
        }

        SDL_DestroyWindow(wnd);
        SDL_Quit();
}

int main(int argc, char**argv)
{
        MakeWindow();
        return 0;
}
