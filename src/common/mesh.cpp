#include "mesh.h"
#include "vertex_attributes.h"

#include "resource_loader.h"
#include "error_codes.h"

#include <GL/glew.h>

#include <iostream>

// TODO: mesh class shouldn't handle loading files, move to renderer once implemented

namespace ORC_NAMESPACE
{

        // TODO: change this method to take care of all attributes
        Mesh::Mesh(const vector<float>& positions, const vector<uint32> indices, const vector<float>& UVs) :
                _count(indices.size())
        {
                glGenVertexArrays(1, &_VAO);
                glGenBuffers(_ATTRIBUTE_COUNT, _VBO);

                glBindVertexArray(_VAO);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _VBO[0]);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) * indices.size(), &indices.front(), GL_STATIC_DRAW);
                
                glBindBuffer(GL_ARRAY_BUFFER, _VBO[VERTEX_POSITION + 1]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * positions.size(), &positions.front(), GL_STATIC_DRAW);
                glVertexAttribPointer(VERTEX_POSITION, 3, GL_FLOAT, GL_FALSE, 0, 0);

                glBindBuffer(GL_ARRAY_BUFFER, _VBO[VERTEX_UV + 1]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(float) * UVs.size(), &UVs.front(), GL_STATIC_DRAW);
                glVertexAttribPointer(VERTEX_UV, 2, GL_FLOAT, GL_FALSE, 0, 0);

                glEnableVertexAttribArray(orc::VERTEX_POSITION);
                glEnableVertexAttribArray(orc::VERTEX_UV);

                glBindVertexArray(0);
        }

        // TODO: delete this method
        Mesh::Mesh(const char* path)
        {
                MeshData data = ResourceLoader::LoadOBJ(path);
                _count = data.indices.size();

                glGenVertexArrays(1, &_VAO);
                glGenBuffers(_ATTRIBUTE_COUNT, _VBO);

                glBindVertexArray(_VAO);

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

                if (data.normals.size() != 0)
                {
                        glBindBuffer(GL_ARRAY_BUFFER, _VBO[VERTEX_NORMAL + 1]);
                        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * data.normals.size(), &data.normals.front(), GL_STATIC_DRAW);
                        glVertexAttribPointer(VERTEX_NORMAL, 3, GL_FLOAT, GL_FALSE, 0, 0);
                        glEnableVertexAttribArray(orc::VERTEX_NORMAL);
                }
                else throw Error::OPENGL_MESH_INVALID;

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