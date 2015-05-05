#ifndef _GENERIC_SHADER_H
#define _GENERIC_SHADER_H

#include "shader.h"

namespace ORC_NAMESPACE
{

        /*
        Generic shader with automated uniform name detection and binding.
        Please note that this class emphasizes correctness & convenience over speed, it's helpful when developing a lot of new shader code.
        When finished it's recommended to implement a specific shader class for it.
        */
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
