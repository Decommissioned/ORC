#include "shader.h"
#include "error_codes.h"
#include "util.h"

#include <GL/glew.h>

#include <string>
#include <fstream>
#include <iostream>

namespace ORC_NAMESPACE
{

        uint32 Shader::CompileShader(const string& source, GLenum type)
        {
                uint32 shaderID;
                GLint success;
                GLint length[2];
                const char* src[2];

                // Define specific preprocessor macros depending on the type of shader being compiled
                if (type == GL_VERTEX_SHADER)
                        src[0] = "#define _VERTEX_SHADER_\n";
                else if (type == GL_FRAGMENT_SHADER)
                        src[0] = "#define _FRAGMENT_SHADER_\n";
                else
                        src[0] = "";

                src[1] = source.c_str();

                length[0] = (GLint) strlen(src[0]);
                length[1] = (GLint) source.length();

                shaderID = glCreateShader(type);
                glShaderSource(shaderID, 2, src, length);
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

        Shader::Shader(const string& vertex_source, const string& fragment_source, std::initializer_list<std::pair<uint8, string>> attributes)
        {
                uint32 vertexID = CompileShader(vertex_source, GL_VERTEX_SHADER);
                uint32 fragmentID = CompileShader(fragment_source, GL_FRAGMENT_SHADER);

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

        void Shader::Bind() const
        {
                glUseProgram(_programID);
                _bound_programID = _programID;
        }

        void Shader::BindAttribute(uint8 slot, const char* attribute)
        {
                glBindAttribLocation(_programID, slot, attribute);
        }

        uint32 Shader::ID() const
        {
                return _programID;
        }

        uint32 Shader::BoundID()
        {
                return _bound_programID;
        }

        THREAD_LOCAL_STORAGE uint32 Shader::_bound_programID = 0;

};
