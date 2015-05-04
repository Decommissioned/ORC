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

#include "../common/generic_shader.h"
#include "../common/mesh.h"
#include "../common/texture.h"

#include "../common/uniform_buffer.h"
#include "../common/entity.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

//////////////////////////////////////////////////////////////////////////

using DM = orc::DisplayManager;

glm::vec3 dragon_pos = {-0.5f, 0.0f, 2.0f};

void MouseMotionHandler(orc::int16 dx, orc::int16 dy)
{
        dragon_pos.x -= dx * 0.01f;
        dragon_pos.z += dy * 0.025f;
}

void Render(orc::uint32 windowID, float r, float g, float b)
{
        DM::CreateOpenGLContext(windowID);

        auto func = DM::MouseMovementCallback(MouseMotionHandler);
        DM::AddMouseMovementHandler(windowID, func);

        glClearColor(r, g, b, 1.0f);

        vector<orc::uint32> indices = {
                0, 1, 2, 2, 3, 0,
                0, 1, 4, 1, 4, 5,
                2, 3, 6, 3, 6, 7,
                0, 3, 4, 3, 4, 7,
                1, 2, 5, 2, 5, 6,
                4, 5, 6, 6, 7, 4,
        };
        vector<float> positions = {
                +1, +1, +1, // 0
                +1, -1, +1, // 1
                -1, -1, +1, // 2
                -1, +1, +1, // 3
                +1, +1, -1, // 4
                +1, -1, -1, // 5
                -1, -1, -1, // 6
                -1, +1, -1, // 7
        };
        vector<float> UVs = {
                0.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f,
                0.0f, 0.0f,
        };

        struct Transformation
        {
                glm::mat4 projection;
                glm::mat4 view;
                ALIGN_AS(16) glm::vec3 eye;
                ALIGN_AS(16) glm::vec3 sun;
                ALIGN_AS(16) glm::vec3 ambient;
                float render_distance;
        };

        Transformation transformation;
        transformation.ambient = { 1.00f, 1.00f, 1.00f };
        transformation.sun = glm::normalize(glm::vec3(1.0f,-1.0f,0.0f));
        transformation.eye = { 0.0f, 0.0f, -1.0f };
        transformation.view =  glm::lookAt(transformation.eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        transformation.projection = glm::perspectiveFov<float>(45.0f, 640.0f, 480.0f, 0.01f, 1000.0f);

        orc::GenericShader shader = orc::GenericShader();
        shader.Bind();

        orc::UniformBuffer ubo(shader.ID(), "global");
        ubo.Update(transformation);
        
        orc::Mesh mesh("monkey.obj");
        //orc::Mesh mesh(positions, indices, UVs);
        mesh.Bind();

        orc::Texture2D texture("marble.png");
        texture.Bind();

        orc::Entity shovel1 = orc::Entity(mesh.ID(), mesh.Count(), shader.ID(), texture.ID());

        PRINT_LAST_ERROR;

        while (!DM::ExitRequested)
        {

                shovel1.transform.Translate(dragon_pos.x, dragon_pos.y, dragon_pos.z);
                shovel1.transform.Rotate(clock() * 0.001f, clock() * 0.001f, clock() * 0.001f);
                shader.SetUniform("model_matrix", shovel1.transform.GetModelMatrix());
                shader.SetUniform("normal_matrix", shovel1.transform.GetNormalMatrix());
                shovel1.transform.LoadIdentity();
                shovel1.Render();

                DM::Present(windowID);
                std::this_thread::sleep_for(std::chrono::microseconds(16667));
        }
        DM::DestroyWindow(windowID);
}

int main(int argc, char**argv)
{
        auto hwnd1 = DM::CreateWindow("Window A", 640, 480);
        std::thread t1(Render, hwnd1, 0.1f, 0.1f, 0.2f);
        //auto hwnd2 = DM::CreateWindow("Window B", 640, 480);
        //std::thread t2(Render, hwnd2, 0.1f, 0.2f, 0.1f);
        //auto hwnd3 = DM::CreateWindow("Window C", 640, 480);
        //std::thread t3(Render, hwnd3, 0.2f, 0.1f, 0.1f);

        DM::EnterMessageLoop();

        t1.join();
        //t2.join();
        //t3.join();

        return 0;
}
