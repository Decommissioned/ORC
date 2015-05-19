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

#include "../common/camera.h"
#include "../common/renderer.h"

//////////////////////////////////////////////////////////////////////////

using DM = orc::DisplayManager;

orc::Renderer* renderer;
orc::Camera camera;

glm::vec3 interest = {0.0f, 0.0f, 0.0f};

void MouseButtonHandler(orc::uint8 button, bool down)
{
        
}

void MouseMotionHandler(orc::int16 dx, orc::int16 dy)
{
        camera.Roll(dx * -0.007f);
        camera.Pitch(dy * 0.004f);
}

void MouseWheelHandler(orc::int16 direction)
{
        camera.IncDistance(float(direction) * 0.25f);
}

void KeyboardHandler(char key, bool down)
{
        glm::vec3 last = interest;

        const float speed = 0.1f;
        switch (key)
        {
        case 'w': interest.z += speed; break;
        case 's': interest.z -= speed; break;
        case 'a': interest.x += speed; break;
        case 'd': interest.x -= speed; break;
        }

        camera.Move(interest - last);
}

void Render(orc::uint32 windowID, float r, float g, float b)
{
        DM::CreateOpenGLContext(windowID);

        renderer = new orc::Renderer();

        DM::AddMouseMovementHandler(windowID, MouseMotionHandler);
        DM::AddMouseWheelHandler(windowID, MouseWheelHandler);
        DM::AddKeyboardHandler(windowID, KeyboardHandler);
        DM::AddMouseButtonHandler(windowID, MouseButtonHandler);

        glClearColor(r, g, b, 1.0f);

        std::vector<orc::Entity*> ents;
        ents.reserve(101);
        for (int i = 0; i < 100; i++)
        {
                ents.emplace_back(&renderer->AddEntity("sphere", "marble"));
        }

        ents.emplace_back(&renderer->AddEntity("monkey", "dome"));
        orc::Entity* player = ents.back();

        PRINT_LAST_ERROR;

        float t= 0.0f;
        while (!DM::ExitRequested)
        {
                //camera.Track(interest);
                renderer->View(camera.View());

                for (int i = 0; i < 100; i++)
                {
                        ents[i]->transform.Translate((-5 + i / 10) * 14 + 2.5, 4 * (sinf(i) + 1.0f), (-5 + i % 10) * 14);
                        ents[i]->transform.Rotate(0.0f, t * (i + 1.0f) / 100.0f, 0.0f);
                }
                auto pos = camera.Position();
                player->transform.Translate(pos.x, pos.y, pos.z);
                player->transform.Rotate(0.0f, camera.Roll(), 0.0f);

                renderer->Update(0.016f);

                for (auto entity : ents)
                {
                        entity->transform.LoadIdentity();
                }
                player->transform.LoadIdentity();

                DM::Present(windowID);
                t += 0.01f;
        }
        DM::DestroyWindow(windowID);

        delete renderer;
}

int main(int argc, char**argv)
{

        auto hwnd1 = DM::CreateWindow("Window A", 1280, 800);
        std::thread t1(Render, hwnd1, 0.1f, 0.1f, 0.2f);
        DM::EnterMessageLoop();
        t1.join();

        return 0;
}
