#include <GL/glew.h>

#include "../common/DisplayManager.h"

#include <thread>
#include <string.h>

#include "../common/error_codes.h"

#define PRINT_LAST_ERROR std::cerr << gluErrorString(glGetError()) << std::endl

//////////////////////////////////////////////////////////////////////////

#if _WIN32
extern void HideConsoleWindow();
#endif

#include <iostream>

#include "../common/renderer.h"

//////////////////////////////////////////////////////////////////////////

using DM = orc::DisplayManager;

void MouseButtonHandler(orc::uint8 button, bool down)
{
        
}

void MouseMotionHandler(orc::int16 dx, orc::int16 dy)
{
        
}

void MouseWheelHandler(orc::int16 m)
{
        
}

void KeyboardHandler(char key, bool down)
{

}

void Render(orc::uint32 windowID, float r, float g, float b)
{
        DM::CreateOpenGLContext(windowID);

        DM::AddMouseMovementHandler(windowID, MouseMotionHandler);
        DM::AddMouseWheelHandler(windowID, MouseWheelHandler);
        DM::AddKeyboardHandler(windowID, KeyboardHandler);
        DM::AddMouseButtonHandler(windowID, MouseButtonHandler);

        glClearColor(r, g, b, 1.0f);

        orc::Renderer renderer;

        std::vector<orc::Entity*> ents;
        ents.reserve(100);
        for (int i = 0; i < 100; i++)
        {
                if (i % 3 == 0) ents.emplace_back(&renderer.AddEntity("monkey", "jade"));
                else if (i % 2 == 0) ents.emplace_back(&renderer.AddEntity("peon", "marble"));
                else ents.emplace_back(&renderer.AddEntity("sphere", "jade"));
        }

        PRINT_LAST_ERROR;

        float t= 0.0f;
        while (!DM::ExitRequested)
        {
                for (int i = 0; i < 100; i++)
                {
                        ents[i]->transform.Translate((-5 + i / 10) * 3, (-5 + i % 10) * 3, 15.0f);
                        ents[i]->transform.Rotate(i * 0.02f, t, t * 2.0f - i * 0.05f);
                }

                renderer.Update(0.016f);

                for (auto entity : ents)
                {
                        entity->transform.LoadIdentity();
                }

                DM::Present(windowID);
                t += 0.01f;
        }
        DM::DestroyWindow(windowID);
}

int main(int argc, char**argv)
{

        auto hwnd1 = DM::CreateWindow("Window A", 1280, 800);
        std::thread t1(Render, hwnd1, 0.1f, 0.1f, 0.2f);
        DM::EnterMessageLoop();
        t1.join();

        return 0;
}
