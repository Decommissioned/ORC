#ifndef _RENDERER_H
#define _RENDERER_H

#include "config.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "mesh.h"
#include "skydome.h"
#include "generic_shader.h"
#include "uniform_buffer.h"
#include "map.h"

#include <deque>

namespace ORC_NAMESPACE
{

        class Renderer
        {

                struct GlobalUniform
                {
                        glm::mat4 projection;
                        glm::mat4 view;
                        ALIGN_AS(16) glm::vec3 eye;
                        ALIGN_AS(16) glm::vec3 ambient;
                        ALIGN_AS(16) glm::vec3 attenuation_factor;
                        glm::vec4 sky_color;
                        float light_damping;
                        float render_distance;
                        float timestamp;
                };

                GlobalUniform m_global;

                std::deque<Mesh> m_meshes;
                std::deque<Texture2D> m_textures;
                std::deque<Entity> m_entities;

                GenericShader gs;
                UniformBuffer ubo;
                Skydome skydome; // TODO: construct on stack

                Map map;

                static std::deque<Mesh> DiscoverAndLoadMeshes();
                static std::deque<Texture2D> DiscoverAndLoadTextures();
                Texture2D& GetTexture(const string& name);
                Mesh& GetMesh(const string& name);

        public:

                Renderer();
                ~Renderer();

                Entity& AddEntity(const string& meshName, const string& textureName);

                void View(const glm::mat4& view);
                void Projection(const glm::mat4& projection);

                void Update(float dt);

        };

};

#endif // _RENDERER_H
