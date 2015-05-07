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

#include "../common/skydome.h"

//////////////////////////////////////////////////////////////////////////

using DM = orc::DisplayManager;
using glm::mat4; using glm::mat3; using glm::mat2;
using glm::vec4; using glm::vec3; using glm::vec2;

vec3 object_position = vec3(0.0f, 0.0f, 13.0f);
bool wireframe = false;
bool move = false;

float z = 1.0f;

struct Transformation
{
        mat4 projection;
        mat4 view;
        ALIGN_AS(16) vec3 eye;
        ALIGN_AS(16) vec3 ambient;
        ALIGN_AS(16) vec3 attenuation_factor;
        float light_damping;
        float render_distance;
        float timestamp;
};

Transformation global;

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
        if (!down) return;

        if (key == 'w') global.eye.z += 0.05f;
        if (key == 's') global.eye.z -= 0.05f;

        if (key == 'd') global.eye.x -= 0.05f;
        if (key == 'a') global.eye.x += 0.05f;

        if (key == '1') z += 1.0f;
        if (key == '2') z -= 1.0f;

        global.view =  glm::lookAt(global.eye, global.eye + glm::vec3(0.0f, 0.0, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Render(orc::uint32 windowID, float r, float g, float b)
{
        DM::CreateOpenGLContext(windowID);

        DM::AddMouseMovementHandler(windowID, MouseMotionHandler);
        DM::AddMouseWheelHandler(windowID, MouseWheelHandler);
        DM::AddKeyboardHandler(windowID, KeyboardHandler);
        DM::AddMouseButtonHandler(windowID, MouseButtonHandler);

        glClearColor(r, g, b, 1.0f);

        global.light_damping = 10.0f;
        global.attenuation_factor = {1.0f, 0.0f, 0.0f};
        global.ambient = {0.2f, 0.2f, 0.2f};
        global.eye = {0.0f, 0.0f, -1.0f};
        global.view =  glm::lookAt(global.eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        global.projection = glm::perspectiveFov<float>(45.0f, 1280.0f, 800.0f, 0.01f, 1000.0f);
        global.render_distance = 1000.0f;

        orc::GenericShader shader = orc::GenericShader();
        orc::Mesh mesh(orc::ResourceLoader::LoadOBJ("dragon.obj"));
        orc::Texture2D jade("jade.png");
        orc::Texture2D clouds("dome.png");
        orc::Entity dragon = orc::Entity(shader, mesh, jade);
        orc::Skydome sky(clouds);

        orc::UniformBuffer ubo({shader.ID(), sky.ShaderID()}, "global");

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
                dragon.transform.Translate(object_position.x, object_position.y, object_position.z);
                dragon.transform.Rotate(0.0f, clock() * 0.001f, 0.0f);
                dragon.Render();

                sky.Draw();
                
                DM::Present(windowID);

                global.timestamp = float(clock()) / float(CLOCKS_PER_SEC);
                ubo.Update(global);
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
