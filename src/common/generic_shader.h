#ifndef _GENERIC_SHADER_H
#define _GENERIC_SHADER_H

#include "shader.h"

namespace ORC_NAMESPACE
{

        class GenericShader : public Shader
        {

        public:

                GenericShader();

                virtual ~GenericShader();

                void SetUniform(const char* name, const void* data);

        protected:

                vector<std::pair<string, uint32>> attributes;

        };

};

#endif // _GENERIC_SHADER_H
