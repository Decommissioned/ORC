#ifndef _SKYDOME_SHADER_H
#define _SKYDOME_SHADER_H

#include "shader.h"
#include "resource_types.h"

namespace ORC_NAMESPACE
{

        class SkydomeShader final : public Shader
        {

        public:

                SkydomeShader();

                SkydomeShader(const SkydomeShader&) = delete;

                ~SkydomeShader();

        };

};

#endif // _SKYDOME_SHADER_H
