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

                struct _uniform
                {
                        string name;
                        int32 location;
                        uint32 type;
                };

                vector<_uniform> attributes;

        };

};

#endif // _GENERIC_SHADER_H
