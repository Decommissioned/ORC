#include "mesh.h"
#include "vertex_attributes.h"

#include "error_codes.h"

#include <GL/glew.h>

namespace ORC_NAMESPACE
{

        Mesh::Mesh(const MeshData& data)
        {
                glGenVertexArrays(1, &_VAO);
                glGenBuffers(_ATTRIBUTE_COUNT, _VBO);

                Bind();

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _VBO[0]);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * data.indices.size(), &data.indices.front(), GL_STATIC_DRAW);

                if (data.positions.size() != 0)
                {
                        glBindBuffer(GL_ARRAY_BUFFER, _VBO[VERTEX_POSITION + 1]);
                        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.positions.size(), &data.positions.front(), GL_STATIC_DRAW);
                        glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
                        glEnableVertexAttribArray(orc::VERTEX_POSITION);
                }
                else throw Error::OPENGL_MESH_INVALID;

                if (data.uvs.size() != 0) // Positions and normals are required, texture coordinates are optional for now
                {
                        glBindBuffer(GL_ARRAY_BUFFER, _VBO[VERTEX_UV + 1]);
                        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.uvs.size(), &data.uvs.front(), GL_STATIC_DRAW);
                        glVertexAttribPointer(VERTEX_UV, 2, GL_FLOAT, GL_FALSE, 0, 0);
                        glEnableVertexAttribArray(orc::VERTEX_UV);
                }
                else glDisableVertexAttribArray(orc::VERTEX_UV);

                if (data.normals.size() != 0)
                {
                        glBindBuffer(GL_ARRAY_BUFFER, _VBO[VERTEX_NORMAL + 1]);
                        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.normals.size(), &data.normals.front(), GL_STATIC_DRAW);
                        glVertexAttribPointer(VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
                        glEnableVertexAttribArray(orc::VERTEX_NORMAL);
                }
                else throw Error::OPENGL_MESH_INVALID;
        }

        Mesh::~Mesh()
        {
                glDeleteBuffers(_ATTRIBUTE_COUNT, _VBO);
                glDeleteVertexArrays(1, &_VAO);
        }

        void Mesh::Bind() const
        {
                glBindVertexArray(_VAO);
                _bound_VAO = _VAO;
        }

        void Mesh::Draw() const
        {
                glDrawElements(GL_TRIANGLES, _count, GL_UNSIGNED_INT, 0);
        }

        uint32 Mesh::Count() const
        {
                return _count;
        }

        uint32 Mesh::ID() const
        {
                return _VAO;
        }

        uint32 Mesh::BoundID()
        {
                return _bound_VAO;
        }

        THREAD_LOCAL_STORAGE uint32 Mesh::_bound_VAO = 0;

};