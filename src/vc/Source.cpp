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

#include "../common/resource_loader.h"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

//////////////////////////////////////////////////////////////////////////

using DM = orc::DisplayManager;
using glm::mat4; using glm::mat3; using glm::mat2;
using glm::vec4; using glm::vec3; using glm::vec2;

vec3 object_position = vec3(0.0f, 0.0f, 13.0f);
bool wireframe = false;
bool move = false;

float z = 1.0f;

void MouseButtonHandler(orc::uint8 button, bool down)
{
        if (button == 1)
        {
                if (down) move = true;
                else move = false;
        }
}

void MouseMotionHandler(orc::int16 dx, orc::int16 dy)
{
        if (move)
        {
                object_position.y -= dy * 0.025f;
                object_position.x -= dx * 0.025f;
        }
}

void MouseWheelHandler(orc::int16 m)
{
        object_position.z += m;
}

void KeyboardHandler(char key, bool down)
{
        if (down && key == 'w') wireframe = !wireframe;

        if (down && key == '1') z += 1.0f;
        if (down && key == '2') z -= 1.0f;
}

void Render(orc::uint32 windowID, float r, float g, float b)
{
        DM::CreateOpenGLContext(windowID);

        DM::AddMouseMovementHandler(windowID, MouseMotionHandler);
        DM::AddMouseWheelHandler(windowID, MouseWheelHandler);
        DM::AddKeyboardHandler(windowID, KeyboardHandler);
        DM::AddMouseButtonHandler(windowID, MouseButtonHandler);

        glClearColor(r, g, b, 1.0f);

        struct Transformation
        {
                mat4 projection;
                mat4 view;
                ALIGN_AS(16) vec3 eye;
                ALIGN_AS(16) vec3 ambient;
                ALIGN_AS(16) vec3 attenuation_factor;
                float light_damping;
                float render_distance;
        };

        Transformation transformation;
        transformation.light_damping = 10.0f;
        transformation.attenuation_factor = {1.0f, 0.0f, 0.0f};
        transformation.ambient = {0.2f, 0.2f, 0.2f};
        transformation.eye = {0.0f, 0.0f, -1.0f};
        transformation.view =  glm::lookAt(transformation.eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        transformation.projection = glm::perspectiveFov<float>(45.0f, 1280.0f, 800.0f, 0.01f, 1000.0f);

        orc::GenericShader shader = orc::GenericShader();

        orc::UniformBuffer ubo({shader.ID()}, "global");
        ubo.Update(transformation);

        orc::Mesh mesh(orc::ResourceLoader::LoadOBJ("dragon.obj"));

        orc::Texture2D jade("jade.png");

        orc::Entity dragon = orc::Entity(shader, mesh, jade);

        vec3 Ka = {0.9f, 0.9f, 0.9f};
        vec3 Kd = {0.5f, 0.5f, 0.5f};
        vec3 Ks = {0.8f, 0.8f, 0.8f};

        vec3 sun = glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f));

        shader.SetUniform("Ka", &Ka);
        shader.SetUniform("Kd", &Kd);
        shader.SetUniform("Ks", &Ks);
        shader.SetUniform("sun", &sun);

        PRINT_LAST_ERROR;

        while (!DM::ExitRequested)
        {
                auto eli = std::chrono::system_clock::now();

                if (wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                dragon.transform.Translate(object_position.x, object_position.y, object_position.z);
                dragon.transform.Rotate(0.0f, clock() * 0.001f, 0.0f);
                dragon.Render();
                
                DM::Present(windowID);
                auto duration = std::chrono::system_clock::now() - eli;
                std::this_thread::sleep_for(std::chrono::microseconds(16667) - std::chrono::duration_cast<std::chrono::microseconds>(duration));
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
