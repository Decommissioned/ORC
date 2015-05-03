#include "generic_shader.h"
#include "error_codes.h"

#include <GL\glew.h>

namespace ORC_NAMESPACE
{

        GenericShader::GenericShader()
                : Shader("vertex.shader", "fragment.shader", {{VERTEX_POSITION, "position"}, {VERTEX_UV, "uv"}})
        {
                // Gets the list of
                int32 count;
                glGetProgramiv(_programID, GL_ACTIVE_UNIFORMS, &count);
                attributes.reserve(count);
                char buffer[255]; int32 length; int32 size; uint32 type;
                for (uint8 i = 0; i < (uint8) count; i++)
                {
                        glGetActiveUniform(_programID, i, 255, &length, &size, &type, buffer);
                        attributes.emplace_back(std::make_pair(buffer, type));
                }
        };

        GenericShader::~GenericShader()
        {}

        void GenericShader::SetUniform(const char* name, const void* data)
        {
                bool found = false;
                uint32 type;
                uint32 location = 0;
                for (auto& attribute : attributes)
                {
                        if (attribute.first == name)
                        {
                                type = attribute.second;
                                found = true;
                                break;
                        }
                        location++;
                }

                if (!found) throw Error::OPENGL_INVALID_UNIFORM_NAME;

                switch (type) // TODO: Think harder about a better way of handling this mess
                {
                case GL_FLOAT: glUniform1f(location, *((float*) data));
                        break;
                case GL_FLOAT_VEC2: glUniform2f(location, *((float*) data), *(((float*) data) + 1));
                        break;
                case GL_FLOAT_VEC3: glUniform3f(location, *((float*) data), *(((float*) data) + 1), *(((float*) data) + 2));
                        break;
                case GL_FLOAT_VEC4: glUniform4f(location, *((float*) data), *(((float*) data) + 1), *(((float*) data) + 2), *(((float*) data) + 3));
                        break;
                case GL_INT: glUniform1i(location, *((int*) data));
                        break;
                case GL_INT_VEC2: glUniform2i(location, *((int*) data), *(((int*) data) + 1));
                        break;
                case GL_INT_VEC3: glUniform3i(location, *((int*) data), *(((int*) data) + 1), *(((int*) data) + 2));
                        break;
                case GL_INT_VEC4: glUniform4i(location, *((int*) data), *(((int*) data) + 1), *(((int*) data) + 2), *(((int*) data) + 3));
                        break;
                case GL_FLOAT_MAT2: glUniformMatrix2fv(location, 1, GL_FALSE, (float*) data);
                        break;
                case GL_FLOAT_MAT3: glUniformMatrix3fv(location, 1, GL_FALSE, (float*) data);
                        break;
                case GL_FLOAT_MAT4: glUniformMatrix4fv(location, 1, GL_FALSE, (float*) data);
                        break;
                default: throw Error::OPENGL_INVALID_UNIFORM_TYPE;
                }
        }

};