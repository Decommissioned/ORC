#ifndef _MAP_SHADER_H
#define _MAP_SHADER_H

#include "shader.h"

#include <glm/vec3.hpp>

namespace ORC_NAMESPACE
{

        class MapShader final : public Shader
        {

                uint32 m_loc_sun;

        public:

                void SetSun(const glm::vec3& direction);

                MapShader();
                MapShader(const MapShader&) = delete;

                ~MapShader();

        };

}

#endif // _MAP_SHADER_H
