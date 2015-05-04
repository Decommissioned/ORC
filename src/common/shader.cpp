#include "shader.h"
#include "error_codes.h"
#include "util.h"

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <iostream>

namespace ORC_NAMESPACE
{

        string Shader::ReadShaderFile(const char* path)
        {
                const string folder = "../shader/";
                return util::ReadFileText(folder + path);
        }

        uint32 Shader::CompileShader(const string& source, GLenum type)
        {
                uint32 shaderID;
                GLint success;
                const char* src = source.c_str();
                int32 length = static_cast<int32>(source.length());

                shaderID = glCreateShader(type);
                glShaderSource(shaderID, 1, &src, &length);
                glCompileShader(shaderID);
                glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

                if (success == GL_FALSE)
                {
                        GLint length = 0;
                        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
                        GLchar* log = new GLchar[length];
                        DEFER(delete[] log);
                        glGetShaderInfoLog(shaderID, length, &length, log);
                        glDeleteShader(shaderID);
                        std::cerr << "Shader compilation failed:\n" << log << std::endl;
                        throw Error::OPENGL_SHADER_COMPILATION;
                }

                return shaderID;
        }


        uint32 Shader::CreateProgram(uint32 vertexID, uint32 fragmentID)
        {
                uint32 programID = glCreateProgram();
                glAttachShader(programID, vertexID);
                glAttachShader(programID, fragmentID);
                return programID;
        }

        void Shader::LinkProgram(uint32 programID)
        {
                GLint success;

                glLinkProgram(programID);
                glGetProgramiv(programID, GL_LINK_STATUS, &success);
                if (success == GL_FALSE)
                {
                        GLint length = 0;
                        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
                        GLchar* log = new GLchar[length];
                        DEFER(delete[] log);
                        glGetProgramInfoLog(programID, length, &length, log);
                        glDeleteProgram(programID);
                        std::cerr << "Shader linking failed:\n" << log << std::endl;
                        throw Error::OPENGL_SHADER_LINKING;
                }

                glValidateProgram(programID);
                glGetProgramiv(programID, GL_VALIDATE_STATUS, &success);
                if (success == GL_FALSE)
                {
                        GLint length = 0;
                        glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &length);
                        GLchar* log = new GLchar[length];
                        DEFER(delete[] log);
                        glGetProgramInfoLog(programID, length, &length, log);
                        glDeleteProgram(programID);
                        std::cerr << "Shader validation failed:\n" << log << std::endl;
                        throw Error::OPENGL_SHADER_VALIDATION;
                }
        }

        void Shader::DeleteShader(uint32 programID, uint32 shaderID)
        {
                glDetachShader(programID, shaderID);
                glDeleteShader(shaderID);
        }

        //////////////////////////////////////////////////////////////////////////

        Shader::Shader(const char* vertex, const char* fragment)
        {
                string vertex_src = ReadShaderFile(vertex);
                string frag_src = ReadShaderFile(fragment);
                uint32 vertexID = CompileShader(vertex_src, GL_VERTEX_SHADER);
                uint32 fragmentID = CompileShader(frag_src, GL_FRAGMENT_SHADER);

                _programID = CreateProgram(vertexID, fragmentID);

                LinkProgram(_programID);
                DeleteShader(_programID, vertexID);
                DeleteShader(_programID, fragmentID);
        }

        Shader::Shader(const char* vertex, const char* fragment, std::initializer_list<std::pair<uint8, string>> attributes)
        {
                string vertex_src = ReadShaderFile(vertex);
                string frag_src = ReadShaderFile(fragment);
                uint32 vertexID = CompileShader(vertex_src, GL_VERTEX_SHADER);
                uint32 fragmentID = CompileShader(frag_src, GL_FRAGMENT_SHADER);

                _programID = CreateProgram(vertexID, fragmentID);

                for (auto& attribute : attributes)
                        BindAttribute(attribute.first, attribute.second.c_str());

                LinkProgram(_programID);
                DeleteShader(_programID, vertexID);
                DeleteShader(_programID, fragmentID);
        }

        Shader::~Shader()
        {
                glDeleteProgram(_programID);
        }

        void Shader::Bind()
        {
                glUseProgram(_programID);
        }

        void Shader::BindAttribute(uint8 slot, const char* attribute)
        {
                glBindAttribLocation(_programID, slot, attribute);
        }

        uint32 Shader::ID() const
        {
                return _programID;
        }

};
