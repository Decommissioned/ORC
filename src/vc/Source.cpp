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

glm::vec3 dragon_position = {0.0f, -4.5f, 13.0f};

using glm::mat4; using glm::mat3; using glm::mat2;
using glm::vec4; using glm::vec3; using glm::vec2;

void MouseMotionHandler(orc::int16 dx, orc::int16 dy)
{

}

void Render(orc::uint32 windowID, float r, float g, float b)
{
        DM::CreateOpenGLContext(windowID);

        auto func = DM::MouseMovementCallback(MouseMotionHandler);
        DM::AddMouseMovementHandler(windowID, func);

        glClearColor(r, g, b, 1.0f);

        struct Transformation
        {
                mat4 projection;
                mat4 view;
                ALIGN_AS(16) vec3 eye;
                ALIGN_AS(16) vec3 ambient;
                ALIGN_AS(16) vec3 attenuation;
                float render_distance;
        };

        Transformation transformation;
        transformation.attenuation = {1.0f, 0.0f, 0.0f};
        transformation.ambient = {0.2f, 0.2f, 0.2f};
        transformation.eye = {0.0f, 0.0f, -1.0f};
        transformation.view =  glm::lookAt(transformation.eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        transformation.projection = glm::perspectiveFov<float>(45.0f, 640.0f, 480.0f, 0.01f, 1000.0f);

        orc::GenericShader shader = orc::GenericShader();
        shader.Bind();

        orc::UniformBuffer ubo(shader.ID(), "global");
        ubo.Update(transformation);

        orc::Mesh mesh("dragon.obj");
        mesh.Bind();

        orc::Texture2D texture("marble.png");
        texture.Bind();

        orc::Entity dragon = orc::Entity(mesh.ID(), mesh.Count(), shader.ID(), texture.ID());

        vec3 Ka = {0.5f, 0.5f, 0.5f};
        vec3 Kd = {0.8f, 0.6f, 0.5f};
        vec3 Ks = {0.6f, 0.5f, 0.8f};
        vec3 sun = {1.0f, 1.0f, 0.0f};

        float reflectivity = 0.50;
        float roughness = 5.0;

        shader.SetUniform("Ka", &Ka);
        shader.SetUniform("Kd", &Kd);
        shader.SetUniform("Ks", &Ks);
        shader.SetUniform("sun", &sun);
        shader.SetUniform("reflectivity", &reflectivity);
        shader.SetUniform("roughness", &roughness);

        PRINT_LAST_ERROR;

        while (!DM::ExitRequested)
        {

                dragon.transform.Translate(dragon_position.x, dragon_position.y, dragon_position.z);
                dragon.transform.Rotate(0.0f, clock() * 0.001f, 0.0f);
                shader.SetUniform("model_matrix", dragon.transform.GetModelMatrix());
                shader.SetUniform("normal_matrix", dragon.transform.GetNormalMatrix());
                dragon.transform.LoadIdentity();

                dragon.Render();

                DM::Present(windowID);
                std::this_thread::sleep_for(std::chrono::microseconds(16667));
        }
        DM::DestroyWindow(windowID);
}

int main(int argc, char**argv)
{
        auto hwnd1 = DM::CreateWindow("Window A", 800, 600);
        std::thread t1(Render, hwnd1, 0.1f, 0.1f, 0.2f);

        DM::EnterMessageLoop();

        t1.join();

        return 0;
}
