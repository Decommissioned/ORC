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

namespace ORC_NAMESPACE
{

        enum : uint8
        {
                VERTEX_POSITIONS = 0,
                VERTEX_INDICES = 1,
        };

};

struct Model
{
        orc::uint32 VAO;
        orc::uint32 VBO[2];
        orc::uint32 Count;
};

Model LoadVertexBufferData(GLfloat* positions, GLuint* indices, orc::uint32 positions_count, orc::uint32 index_count)
{
        Model model;
        model.Count = index_count;
        glGenVertexArrays(1, &model.VAO);

        glBindVertexArray(model.VAO);
        
        // Positions + indices
        glGenBuffers(2, model.VBO);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.VBO[orc::VERTEX_INDICES]);
        glBindBuffer(GL_ARRAY_BUFFER, model.VBO[orc::VERTEX_POSITIONS]);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * index_count, indices, GL_STATIC_DRAW);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * positions_count, positions, GL_STATIC_DRAW);

        glVertexAttribPointer(orc::VERTEX_POSITIONS, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glBindVertexArray(0);
        return model;
}

#include <iostream>

//////////////////////////////////////////////////////////////////////////

using DM = orc::DisplayManager;

void Render(orc::uint32 windowID, float r, float g, float b)
{
        DM::CreateOpenGLContext(windowID);
        
        glClearColor(r, g, b, 1.0f);

        GLuint indices[] = {
                0, 1, 2,
                1, 3, 4,
                2, 4, 5
        };
        GLfloat positions[] = {
                 0.00f,  0.75f, 0.0f,
                 0.25f,  0.25f, 0.0f,
                -0.25f,  0.25f, 0.0f,
                 0.50f, -0.25f, 0.0f,
                 0.00f, -0.25f, 0.0f,
                -0.50f, -0.25f, 0.0f
        };
        
        Model model = LoadVertexBufferData(positions, indices, 6 * 3, 9);
        PRINT_LAST_ERROR;

        orc::Shader shader = orc::Shader("vertex.shader", "fragment.shader");
        shader.Bind();
        
        while (!DM::ExitRequested)
        {
                
                glClear(GL_COLOR_BUFFER_BIT);

                glBindVertexArray(model.VAO);

                glEnableVertexAttribArray(orc::VERTEX_POSITIONS);
                glDrawElements(GL_TRIANGLES, model.Count, GL_UNSIGNED_INT, 0);

                glBindVertexArray(0);

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
