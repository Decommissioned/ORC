#include "mesh.h"
#include "vertex_attributes.h"

#include "error_codes.h"

#include <GL/glew.h>

namespace ORC_NAMESPACE
{

        Mesh::Mesh(const MeshData& data)
        {
                m_count = data.indices.size();
                m_nameID = data.nameID;

                glGenVertexArrays(1, &m_VAO);
                glGenBuffers(m_ATTRIBUTE_COUNT, m_VBO);

                Bind();

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBO[0]);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * data.indices.size(), &data.indices.front(), GL_STATIC_DRAW);

                if (data.positions.size() != 0)
                {
                        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VERTEX_POSITION + 1]);
                        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.positions.size(), &data.positions.front(), GL_STATIC_DRAW);
                        glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);
                        glEnableVertexAttribArray(orc::VERTEX_POSITION);
                }
                else throw Error::OPENGL_MESH_INVALID;

                if (data.uvs.size() != 0) // Positions and normals are required, texture coordinates are optional for now
                {
                        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VERTEX_UV + 1]);
                        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.uvs.size(), &data.uvs.front(), GL_STATIC_DRAW);
                        glVertexAttribPointer(VERTEX_UV, 2, GL_FLOAT, GL_FALSE, 0, 0);
                        glEnableVertexAttribArray(orc::VERTEX_UV);
                }
                else glDisableVertexAttribArray(orc::VERTEX_UV);

                if (data.normals.size() != 0)
                {
                        glBindBuffer(GL_ARRAY_BUFFER, m_VBO[VERTEX_NORMAL + 1]);
                        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.normals.size(), &data.normals.front(), GL_STATIC_DRAW);
                        glVertexAttribPointer(VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
                        glEnableVertexAttribArray(orc::VERTEX_NORMAL);
                }
                else glDisableVertexAttribArray(orc::VERTEX_NORMAL);
        }

        Mesh::~Mesh()
        {
                glDeleteBuffers(m_ATTRIBUTE_COUNT, m_VBO);
                glDeleteVertexArrays(1, &m_VAO);
        }

        void Mesh::Bind() const
        {
                glBindVertexArray(m_VAO);
                m_bound_VAO = m_VAO;
        }

        void Mesh::Draw() const
        {
                glDrawElements(GL_TRIANGLES, (GLsizei) m_count, GL_UNSIGNED_INT, 0);
        }

        size_t Mesh::Count() const
        {
                return m_count;
        }

        uint32 Mesh::ID() const
        {
                return m_VAO;
        }

        uint32 Mesh::BoundID()
        {
                return m_bound_VAO;
        }

        const string& Mesh::NameID() const
        {
                return m_nameID;
        }

        THREAD_LOCAL_STORAGE uint32 Mesh::m_bound_VAO = 0;

};