#include "generic_shader.h"

namespace ORC_NAMESPACE
{

        GenericShader::GenericShader()
                : Shader("vertex.shader", "fragment.shader")
        {
                BindAttribute(VERTEX_POSITION, "position");
                BindAttribute(VERTEX_UV, "uv");
        }

        GenericShader::~GenericShader()
        {
        }

};