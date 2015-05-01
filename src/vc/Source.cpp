#include <SDL2/SDL.h>
#include <GL/glew.h>

#include "../common/display.h"

int main(int argc, char**argv)
{

        orc::Display d("TEST MESSAGE", 800, 600);
        glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

        SDL_Event e;
        bool quit = false;
        while (!quit)
        {
                if (SDL_PollEvent(&e))
                {
                        quit = (e.type == SDL_QUIT);
                }
                else
                {
                        glClear(GL_COLOR_BUFFER_BIT);
                        d.Present();
                }
        }
        
        return 0;
}
