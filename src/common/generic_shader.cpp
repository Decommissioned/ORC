#include "generic_shader.h"

namespace ORC_NAMESPACE
{

        GenericShader::GenericShader()
                : Shader("vertex.shader", "fragment.shader",
                {std::make_pair(VERTEX_POSITION,"position"), std::make_pair(VERTEX_UV, "uv")})
        {}

        GenericShader::~GenericShader()
        {
        }

};