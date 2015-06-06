#ifndef _SHADER_H
#define _SHADER_H

#include "config.h"

namespace ORC_NAMESPACE
{
        /*
        Shader
        Compiles, binds and links the provided shaders into the GPU, which can then be executed while rendering
        */
        class Shader
        {
        public:

                virtual ~Shader();

                void Bind() const;

                uint32 ID() const;
                static uint32 BoundID();

        protected:

                explicit Shader(const string& vertex_source, const string& fragment_source, std::initializer_list<std::pair<uint8, string>>);

                void BindAttribute(uint8 slot, const char* attribute);

                static uint32 CompileShader(const string& source, uint32 type);
                static uint32 CreateProgram(uint32 vertexID, uint32 fragmentID);
                static void LinkProgram(uint32 programID);
                static void DeleteShader(uint32 programID, uint32 shaderID);

                uint32 m_programID;

                THREAD_LOCAL_STORAGE static uint32 m_bound_programID;

        };

};

#endif // _SHADER_H
