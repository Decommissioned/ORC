#include "entity.h"

#include <GL/glew.h>

namespace ORC_NAMESPACE
{

        Entity::Entity(uint32 mesh, uint32 vertices, uint32 program, uint32 texture)
                : _mesh(mesh), _vertices(vertices), _program(program), _texture(texture)
        {}

        Entity::~Entity()
        {}

        void Entity::Render() // TODO: delete this test method
        {
                int32 boundVAO;
                int32 boundProgram;
                int32 boundTexture;

                glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &boundVAO);
                glGetIntegerv(GL_CURRENT_PROGRAM, &boundProgram);
                glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);

                if (boundVAO != _mesh)
                        glBindVertexArray(_mesh);
                if (boundProgram != _program)
                        glUseProgram(_program);
                if (boundTexture != _texture)
                        glBindTexture(GL_TEXTURE_2D, _texture);

                glDrawElements(GL_TRIANGLES, _vertices, GL_UNSIGNED_INT, 0);
        }

};