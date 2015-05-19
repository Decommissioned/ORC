#include "map.h"

#include <random>

namespace ORC_NAMESPACE
{

        Map::Map(uint64 seed, uint32 width, uint32 height, float spacing, const Texture2D& texture) :
                m_mesh(init_mesh_data_aux(seed, width, height, spacing)), m_texture(texture)
        {
        }

        MeshData& Map::init_mesh_data_aux(uint64 seed, uint32 width, uint32 height, float spacing)
        {
                std::mt19937 mt(seed);
                std::uniform_real_distribution<float> dist(-3.5f, -3.0f);

                m_data.positions.reserve(3 * width * height);
                m_data.indices.reserve(6 * (width - 1) * (height - 1));

                float x = spacing * float(width)  * -0.5f;
                for (uint32 i = 0; i < width; ++i, x+=spacing)
                {
                        float z = spacing * float(height) * -0.5f;
                        for (uint32 j = 0; j < height; ++j, z+=spacing)
                        {
                                float y = dist(mt);
                                m_data.positions.push_back(x);
                                m_data.positions.push_back(y);
                                m_data.positions.push_back(z);
                        }
                }

                for (uint32 i = 0; i < (width - 1); ++i)
                {
                        uint32 offset = i * height;
                        for (uint32 j = 0; j < (height - 1); ++j)
                        {

                                m_data.indices.push_back(offset + j + height);
                                m_data.indices.push_back(offset + j);
                                m_data.indices.push_back(offset + j + 1);

                                m_data.indices.push_back(offset + j + height);
                                m_data.indices.push_back(offset + j + 1);
                                m_data.indices.push_back(offset + j + height + 1);
                        }
                }

                return m_data;
        }

        void Map::Render() const
        {

                if (Shader::BoundID() != m_shader.ID())
                        m_shader.Bind();

                if (Mesh::BoundID() != m_mesh.ID())
                        m_mesh.Bind();

                if (Texture2D::BoundID() != m_texture.ID())
                        m_texture.Bind();

                m_mesh.Draw();
        }

        uint32 Map::ShaderID() const
        {
                return m_shader.ID();
        }

        // TODO: move shader out of class
        void Map::SetSun(const glm::vec3& direction)
        {
                m_shader.Bind();
                m_shader.SetSun(direction);
        }

};
