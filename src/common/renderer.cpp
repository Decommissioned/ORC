#include "renderer.h"

#include "util.h"
#include "error_codes.h"

#include "resource_loader.h"

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

namespace ORC_NAMESPACE
{

        Renderer::Renderer() :
                m_meshes(DiscoverAndLoadMeshes()), m_textures(DiscoverAndLoadTextures()),
                gs(), ubo(), map(2402534U, 800, 800, 0.25f, GetTexture("grass")), skydome(GetTexture("dome"))
        {
                //skydome = new Skydome(GetTexture("dome"));
                ubo.AddPrograms({gs.ID(), skydome.ShaderID(), map.ShaderID()}, "global");

                global.sky_color = {0.6f, 0.6f, 0.6f, 1.0f};
                global.light_damping = 10.0f;
                global.attenuation_factor = {1.0f, 0.0f, 0.0f};
                global.ambient = {0.2f, 0.2f, 0.2f};
                global.eye = {0.0f, 0.0f, -1.0f};
                global.view =  glm::lookAt(global.eye, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                global.projection = glm::perspectiveFov(45.0f, 1280.0f, 800.0f, 0.01f, 1000.0f);
                global.render_distance = 1000.0f;

                glm::vec3 Ka = {0.9f, 0.9f, 0.9f};
                glm::vec3 Kd = {0.5f, 0.5f, 0.5f};
                glm::vec3 Ks = {0.8f, 0.8f, 0.8f};
                glm::vec3 sun = glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f));

                gs.SetUniform("Ka", &Ka);
                gs.SetUniform("Kd", &Kd);
                gs.SetUniform("Ks", &Ks);
                gs.SetUniform("sun", &sun);
                
        }

        Renderer::~Renderer()
        {
                //delete skydome;
        }

        Entity& Renderer::AddEntity(const string& meshName, const string& textureName)
        {
                auto& mesh = GetMesh(meshName);
                auto& texture = GetTexture(textureName);
                m_entities.emplace_back(gs, mesh, texture);
                return m_entities.back();
        }

        void Renderer::Update(float dt)
        {
                global.timestamp += dt;
                ubo.Update(global);

                for (auto& entity : m_entities)
                        entity.Render();

                map.Render();

                skydome.Draw();
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

};