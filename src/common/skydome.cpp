#include "skydome.h"

#include "resource_loader.h"

namespace ORC_NAMESPACE
{

        Skydome::Skydome(const Texture2D& texture) :
                m_dome_shader(SkydomeShader()),
                m_dome(ResourceLoader::LoadOBJ(ORC_MESH_FOLDER_RELATIVE_PATH"skydome.obj")),
                m_texture(texture)
        {
        }

        Skydome::~Skydome()
        {}

        void Skydome::BindIfNeeded() const
        {
                if (Shader::BoundID() != m_dome_shader.ID())
                        m_dome_shader.Bind();

                if (Mesh::BoundID() != m_dome.ID())
                        m_dome.Bind();

                if (Texture2D::BoundID() != m_texture.ID())
                        m_texture.Bind();
        }

        void Skydome::Draw() const
        {
                BindIfNeeded();
                m_dome.Draw();
        }

        uint32 Skydome::ShaderID() const
        {
                return m_dome_shader.ID();
        }

};