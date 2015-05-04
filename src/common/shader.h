#ifndef _SHADER_H
#define _SHADER_H

#include "config.h"

namespace ORC_NAMESPACE
{

        class Shader
        {
        public:

                virtual ~Shader();

                void Bind();

                uint32 ID() const;

        protected:

                explicit Shader(const char* vertex, const char* fragment);
                explicit Shader(const char* vertex, const char* fragment, std::initializer_list<std::pair<uint8, string>>);

                void BindAttribute(uint8 slot, const char* attribute);

                static string ReadShaderFile(const char* path);
                static uint32 CompileShader(const string& source, uint32 type);
                static uint32 CreateProgram(uint32 vertexID, uint32 fragmentID);
                static void LinkProgram(uint32 programID);
                static void DeleteShader(uint32 programID, uint32 shaderID);

                uint32 _programID;

        };

};

#endif // _SHADER_H
