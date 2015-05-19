#ifndef _MAP_H
#define _MAP_H

#include "resource_types.h"
#include "texture.h"
#include "map_shader.h"
#include "mesh.h"

namespace ORC_NAMESPACE
{

        class Map final
        {

                MeshData m_data;
                Mesh m_mesh;
                MapShader m_shader;
                const Texture2D& m_texture;

                MeshData& init_mesh_data_aux(uint64 seed, uint32 width, uint32 height, float spacing);


        public:

                void SetSun(const glm::vec3& direction);

                void Render() const;

                uint32 ShaderID() const;

                Map() = delete;
                Map(uint64 seed, uint32 width, uint32 height, float spacing, const Texture2D& texture);

        };

};

#endif // _MAP_H
