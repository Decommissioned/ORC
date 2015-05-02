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

#include "../common/shader.h"
#include "../common/mesh.h"

#include <iostream>

//////////////////////////////////////////////////////////////////////////

using DM = orc::DisplayManager;

void Render(orc::uint32 windowID, float r, float g, float b)
{
        DM::CreateOpenGLContext(windowID);
        
        glClearColor(r, g, b, 1.0f);

        vector<orc::uint32> indices = {
                0, 1, 2,
                1, 3, 4,
                2, 4, 5
        };
        vector<float> positions = {
                 0.00f,  0.75f, 0.0f,
                 0.25f,  0.25f, 0.0f,
                -0.25f,  0.25f, 0.0f,
                 0.50f, -0.25f, 0.0f,
                 0.00f, -0.25f, 0.0f,
                -0.50f, -0.25f, 0.0f
        };
        orc::Mesh mesh(positions, indices);
        mesh.Bind();
        
        
        PRINT_LAST_ERROR;

        orc::Shader shader = orc::Shader("vertex.shader", "fragment.shader");
        shader.Bind();

        glEnableVertexAttribArray(0);
        
        while (!DM::ExitRequested)
        {
                
                glClear(GL_COLOR_BUFFER_BIT);

                mesh.Draw();

                DM::Present(windowID);
                std::this_thread::sleep_for(std::chrono::microseconds(16667));
                
        }
        DM::DestroyWindow(windowID);
}

int main(int argc, char**argv)
{

        auto hwnd1 = DM::CreateWindow("Window A", 640, 480);
        std::thread t1(Render, hwnd1, 0.1f, 0.1f, 0.2f);
        // auto hwnd2 = DM::CreateWindow("Window B", 640, 480);
        // std::thread t2(Render, hwnd2, 0.1f, 0.2f, 0.1f);
        // auto hwnd3 = DM::CreateWindow("Window C", 640, 480);
        // std::thread t3(Render, hwnd3, 0.2f, 0.1f, 0.1f);

        DM::EnterMessageLoop();

        t1.join();
        // t2.join();
        // t3.join();

        return 0;
}
