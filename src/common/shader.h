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
                void BindAttribute(uint8 slot, const char* attribute);

        protected:

                static string ReadFile(const char* path);
                static uint32 CompileShader(const string& source, uint32 type);
                static uint32 LinkShader(uint32 vertexID, uint32 fragmentID);

                uint32 _programID;

        };

};

#endif // _SHADER_H
