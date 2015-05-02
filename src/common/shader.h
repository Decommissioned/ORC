#ifndef _SHADER_H
#define _SHADER_H

#include "config.h"

namespace ORC_NAMESPACE
{

        class Shader
        {
        public:

                explicit Shader(const char* vertex, const char* fragment);
                virtual ~Shader();

                void Bind();

        private:

                uint32 _programID;

        };

};

#endif // _SHADER_H
