#include "entity.h"

#include <GL/glew.h>

#include <iostream>

namespace ORC_NAMESPACE
{

        Entity::Entity(GenericShader& shader, const Mesh& mesh, const Texture2D& texture)
                : _mesh(mesh), _shader(shader), _texture(texture)
        {}

        Entity::~Entity()
        {}

        void Entity::Render()
        {

                if (Shader::BoundID() != _shader.ID())
                        _shader.Bind();

                if (Mesh::BoundID() != _mesh.ID())
                        _mesh.Bind();

                if (Texture2D::BoundID() != _texture.ID())
                        _texture.Bind();

                _shader.SetUniform("model_matrix", transform.GetModelMatrix());
                _shader.SetUniform("normal_matrix", transform.GetNormalMatrix());

                glDrawElements(GL_TRIANGLES, (GLsizei) _mesh.Count(), GL_UNSIGNED_INT, 0);
                
                transform.LoadIdentity();
        }

};