#ifndef _MAP_SHADER_H
#define _MAP_SHADER_H

#include "shader.h"

namespace ORC_NAMESPACE
{

        class MapShader final : public Shader
        {

        public:

                MapShader();
                MapShader(const MapShader&) = delete;

                ~MapShader();

        };

}

#endif // _MAP_SHADER_H
