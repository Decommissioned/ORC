#include "renderer.h"

#include "util.h"
#include "error_codes.h"

#include "resource_loader.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include <GL/glew.h>

namespace ORC_NAMESPACE
{

        Renderer::Renderer() :
                m_meshes(DiscoverAndLoadMeshes()), m_textures(DiscoverAndLoadTextures()),
                gs(), ubo(), map(2402534U, 800, 800, 2.5f, GetTexture("grass")), skydome(GetTexture("dome"))
        {
                ubo.AddPrograms({gs.ID(), skydome.ShaderID(), map.ShaderID()}, "global");

                m_global.sky_color = {0.6f, 0.6f, 0.6f, 1.0f};
                m_global.light_damping = 10.0f;
                m_global.attenuation_factor = {1.0f, 0.0f, 0.0f};
                m_global.ambient = {0.2f, 0.2f, 0.2f};
                m_global.eye = {0.0f, 0.0f, -1.0f};
                m_global.view =  glm::lookAt(m_global.eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                m_global.projection = glm::perspectiveFov(45.0f, 1280.0f, 800.0f, 0.01f, 1000.0f);
                m_global.render_distance = 1000.0f;

                glm::vec3 Ka = {0.9f, 0.9f, 0.9f};
                glm::vec3 Kd = {0.5f, 0.5f, 0.5f};
                glm::vec3 Ks = {0.8f, 0.8f, 0.8f};
                glm::vec3 sun = glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f));

                gs.SetUniform("Ka", &Ka);
                gs.SetUniform("Kd", &Kd);
                gs.SetUniform("Ks", &Ks);
                gs.SetUniform("sun", &sun);

                map.SetSun(sun);
                
        }

        Renderer::~Renderer()
        {}

        Entity& Renderer::AddEntity(const string& meshName, const string& textureName)
        {
                auto& mesh = GetMesh(meshName);
                auto& texture = GetTexture(textureName);
                m_entities.emplace_back(gs, mesh, texture);
                return m_entities.back();
        }

        void Renderer::Update(float dt)
        {
                m_global.timestamp += dt;
                ubo.Update(m_global);

                glDisable(GL_CULL_FACE);
                glDisable(GL_DEPTH_TEST);
                skydome.Draw();

                glEnable(GL_CULL_FACE);
                glEnable(GL_DEPTH_TEST);
                for (auto& entity : m_entities)
                        entity.Render();

                map.Render();
        }

        Texture2D& Renderer::GetTexture(const string& name)
        {
                for (auto& texture : m_textures)
                {
                        if (texture.NameID() == name)
                                return texture;
                }
                throw Error::RESOURCE_NOT_FOUND;
        }

        Mesh& Renderer::GetMesh(const string& name)
        {
                for (auto& mesh : m_meshes)
                {
                        if (mesh.NameID() == name)
                                return mesh;
                }
                throw Error::RESOURCE_NOT_FOUND;
        }

        std::deque<Texture2D> Renderer::DiscoverAndLoadTextures()
        {
                std::deque<Texture2D> textures;
                vector<string> textures_files = util::ListDirectoryFiles(ORC_TEXTURE_FOLDER_RELATIVE_PATH, ".png");

                for (auto& texture_file : textures_files)
                {
                        std::cout << "Loading texture " << texture_file << std::endl;

                        ImageData img = ResourceLoader::LoadPNG(texture_file);
                        textures.emplace_back(img);
                }
                return textures;
        }

        std::deque<Mesh> Renderer::DiscoverAndLoadMeshes()
        {
                std::deque<Mesh> meshes;
                vector<string> mesh_files = util::ListDirectoryFiles(ORC_MESH_FOLDER_RELATIVE_PATH, ".obj");

                for (auto& mesh_file : mesh_files)
                {
                        std::cout << "Loading mesh " << mesh_file << std::endl;

                        MeshData md = ResourceLoader::LoadOBJ(mesh_file);
                        meshes.emplace_back(md);
                }
                return meshes;
        }

        void Renderer::View(const glm::vec3& eye, const glm::vec3& lookat, const glm::vec3& up)
        {
                m_global.view = glm::lookAt(eye, lookat, up);
                m_global.eye = eye;
        }

        void Renderer::Projection(const glm::mat4& projection)
        {
                m_global.projection = projection;
        }

};