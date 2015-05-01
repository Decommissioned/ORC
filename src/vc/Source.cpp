#include <GL/glew.h>

#include "../common/DisplayManager.h"

#include <thread>

using DM = orc::DisplayManager;

void Render(orc::uint32 windowID, float r, float g, float b)
{
        DM::CreateOpenGLContext(windowID);

        glClearColor(r, g, b, 1.0f);

        while (!DM::ExitRequested)
        {
                glClear(GL_COLOR_BUFFER_BIT);
                DM::Present(windowID);
                std::this_thread::sleep_for(std::chrono::microseconds(16667));
        }
        DM::DestroyWindow(windowID);
}

int main(int argc, char**argv)
{
        auto hwnd1 = DM::CreateWindow("Window A", 640, 480);
        std::thread t1(Render, hwnd1, 0.1f, 0.1f, 0.2f);

        auto hwnd2 = DM::CreateWindow("Window B", 640, 480);
        std::thread t2(Render, hwnd2, 0.1f, 0.2f, 0.1f);

        auto hwnd3 = DM::CreateWindow("Window C", 640, 480);
        std::thread t3(Render, hwnd3, 0.2f, 0.1f, 0.1f);

        DM::EnterMessageLoop();

        t1.join();
        t2.join();
        t3.join();

        return 0;
}
