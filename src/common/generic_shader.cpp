#include "generic_shader.h"
#include "error_codes.h"
#include "vertex_attributes.h"

#include <GL\glew.h>

namespace ORC_NAMESPACE
{

        GenericShader::GenericShader()
                : Shader("vertex.shader", "fragment.shader", {{VERTEX_POSITION, "position"}, {VERTEX_UV, "uv"}, {VERTEX_NORMAL, "normal"}})
        {
                // Gets the list of uniform variable's names in the current program
                int32 count;
                glGetProgramiv(_programID, GL_ACTIVE_UNIFORMS, &count);
                attributes.reserve(count);
                char buffer[255]; int32 length; int32 size; uint32 type;
                for (int32 i = 0; i < count; i++)
                {
                        // Uniform blocks are also included in the above call, fortunately if we query for their location we get -1
                        // Since uniform blocks are stored in a different location, using glUniform* to set them is undefined behavior
                        glGetActiveUniform(_programID, i, 255, &length, &size, &type, buffer);
                        int32 location = glGetUniformLocation(_programID, buffer);

                        if (location != -1)
                                attributes.emplace_back(_uniform{ buffer, location, type });
                }
        };

        GenericShader::~GenericShader()
        {}

        void GenericShader::SetUniform(const char* name, const void* data)
        {
                for (auto& attribute : attributes)
                {
                        if (attribute.name == name)
                        {
                                switch (attribute.type) // TODO: Think harder about a better way of handling this mess
                                {
                                case GL_FLOAT: glUniform1f(attribute.location, *((float*) data));
                                        break;
                                case GL_FLOAT_VEC2: glUniform2f(attribute.location, *((float*) data), *(((float*) data) + 1));
                                        break;
                                case GL_FLOAT_VEC3: glUniform3f(attribute.location, *((float*) data), *(((float*) data) + 1), *(((float*) data) + 2));
                                        break;
                                case GL_FLOAT_VEC4: glUniform4f(attribute.location, *((float*) data), *(((float*) data) + 1), *(((float*) data) + 2), *(((float*) data) + 3));
                                        break;
                                case GL_INT: glUniform1i(attribute.location, *((int*) data));
                                        break;
                                case GL_INT_VEC2: glUniform2i(attribute.location, *((int*) data), *(((int*) data) + 1));
                                        break;
                                case GL_INT_VEC3: glUniform3i(attribute.location, *((int*) data), *(((int*) data) + 1), *(((int*) data) + 2));
                                        break;
                                case GL_INT_VEC4: glUniform4i(attribute.location, *((int*) data), *(((int*) data) + 1), *(((int*) data) + 2), *(((int*) data) + 3));
                                        break;
                                case GL_FLOAT_MAT2: glUniformMatrix2fv(attribute.location, 1, GL_FALSE, (float*) data);
                                        break;
                                case GL_FLOAT_MAT3: glUniformMatrix3fv(attribute.location, 1, GL_FALSE, (float*) data);
                                        break;
                                case GL_FLOAT_MAT4: glUniformMatrix4fv(attribute.location, 1, GL_FALSE, (float*) data);
                                        break;
                                default: throw Error::OPENGL_INVALID_UNIFORM_TYPE;
                                }
                                return;
                        }
                }
                throw Error::OPENGL_INVALID_UNIFORM_NAME;
                
        }

};