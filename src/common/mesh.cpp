#include "mesh.h"
#include "shader_slots.h"

#include <GL/glew.h>

namespace ORC_NAMESPACE
{

        Mesh::Mesh(const vector<float>& positions, const vector<uint32> indices, const vector<float>& UVs) :
                _count(indices.size())
        {
                glGenVertexArrays(1, &_VAO);
                glGenBuffers(_ATTRIBUTE_COUNT, _VBO);

                glBindVertexArray(_VAO);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _VBO[0]);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * indices.size(), &indices[0], GL_STATIC_DRAW);
                
                glBindBuffer(GL_ARRAY_BUFFER, _VBO[VERTEX_POSITION + 1]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(), &positions[0], GL_STATIC_DRAW);
                glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);

                glBindBuffer(GL_ARRAY_BUFFER, _VBO[VERTEX_UV + 1]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * UVs.size(), &UVs[0], GL_STATIC_DRAW);
                glVertexAttribPointer(VERTEX_UV, 2, GL_FLOAT, GL_FALSE, 0, 0);

                glBindVertexArray(0);
        }

        Mesh::~Mesh()
        {
                glDeleteBuffers(_ATTRIBUTE_COUNT, _VBO);
                glDeleteVertexArrays(1, &_VAO);
        }

        void Mesh::Bind()
        {
                glBindVertexArray(_VAO);
                glEnableVertexAttribArray(orc::VERTEX_POSITION);
                glEnableVertexAttribArray(orc::VERTEX_UV);
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

};