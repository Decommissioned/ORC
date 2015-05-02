#include "mesh.h"
#include "shader_slots.h"

#include <GL/glew.h>

namespace ORC_NAMESPACE
{

        Mesh::Mesh(const vector<float>& positions, const vector<uint32> indices) :
                _count(indices.size())
        {
                glGenVertexArrays(1, &_VAO);
                glGenBuffers(2, _VBO);

                glBindVertexArray(_VAO);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _VBO[VERTEX_INDICES]);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * indices.size(), &indices[0], GL_STATIC_DRAW);

                glBindBuffer(GL_ARRAY_BUFFER, _VBO[VERTEX_POSITIONS]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(), &positions[0], GL_STATIC_DRAW);

                glVertexAttribPointer(VERTEX_POSITIONS, 3, GL_FLOAT, GL_FALSE, 0, 0);

                glBindVertexArray(0);
        }

        Mesh::~Mesh()
        {
                glDeleteBuffers(2, _VBO);
                glDeleteVertexArrays(1, &_VAO);
        }

        void Mesh::Bind()
        {
                glBindVertexArray(_VAO);
        }

        void Mesh::Draw() const
        {
                glDrawElements(GL_TRIANGLES, _count, GL_UNSIGNED_INT, 0);
        }

};