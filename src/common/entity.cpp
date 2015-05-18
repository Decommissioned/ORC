#include "entity.h"

namespace ORC_NAMESPACE
{

        Entity::Entity(GenericShader& shader, const Mesh& mesh, const Texture2D& texture)
                : m_mesh(mesh), m_shader(shader), m_texture(texture)
        {}

        Entity::~Entity()
        {}

        void Entity::Render() const
        {

                if (Shader::BoundID() != m_shader.ID())
                        m_shader.Bind();

                if (Mesh::BoundID() != m_mesh.ID())
                        m_mesh.Bind();

                if (Texture2D::BoundID() != m_texture.ID())
                        m_texture.Bind();

                m_shader.SetUniform("model_matrix", transform.GetModelMatrix());
                m_shader.SetUniform("normal_matrix", transform.GetNormalMatrix());

                m_mesh.Draw();
        }

};