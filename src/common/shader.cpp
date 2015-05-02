#include "shader.h"
#include "error_codes.h"

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <iostream>

namespace ORC_NAMESPACE
{

        string Shader::ReadFile(const char* path)
        {
                const string folder = "../shader/";
                std::ifstream t(folder + path);
                if (t.fail()) throw Error::ORC_SHADER_FILE_NOT_FOUND;
                t.seekg(0, std::ios::end);
                size_t size = t.tellg();
                string buffer(size, ' ');
                t.seekg(0);
                t.read(&buffer[0], size);
                return buffer;
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

        uint32 Shader::LinkShader(uint32 vertexID, uint32 fragmentID)
        {
                GLint success;
                uint32 programID = glCreateProgram();

                glAttachShader(programID, vertexID);
                glAttachShader(programID, fragmentID);

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

                glDetachShader(programID, vertexID);
                glDetachShader(programID, fragmentID);
                glDeleteShader(vertexID);
                glDeleteShader(fragmentID);

                return programID;
        }

        //////////////////////////////////////////////////////////////////////////

        Shader::Shader(const char* vertex, const char* fragment)
        {
                string vertex_src = ReadFile(vertex);
                string frag_src = ReadFile(fragment);
                uint32 vertexID = CompileShader(vertex_src, GL_VERTEX_SHADER);
                uint32 fragmentID = CompileShader(frag_src, GL_FRAGMENT_SHADER);
                _programID = LinkShader(vertexID, fragmentID);
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
