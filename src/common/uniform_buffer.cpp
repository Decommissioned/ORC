#include "uniform_buffer.h"
#include "error_codes.h"

#include <GL/glew.h>

namespace ORC_NAMESPACE
{

        UniformBuffer::UniformBuffer(std::initializer_list<uint32> programIDs, const string& name)
        {
                
                if (_max_binding_points == 0) // Thread "singleton" variable
                {
                        int32 max;
                        glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &max);

                        if (max <= 0) throw Error::OPENGL_UNIFORM_BUFFER_QUERY;

                        _max_binding_points = max;
                }

                if (_next_binding_point >= _max_binding_points)
                        throw Error::OPENGL_UNIFORM_BUFFER_FULL;

                _binding_point = _next_binding_point;
                _next_binding_point++;

                for (auto programID : programIDs)
                {
                        uint32 blockID = glGetUniformBlockIndex(programID, name.c_str());
                        glUniformBlockBinding(programID, blockID, _binding_point);
                }

                glGenBuffers(1, &_bufferID);
        }

        UniformBuffer::~UniformBuffer()
        {
                glDeleteBuffers(1, &_bufferID);
        }

        void UniformBuffer::_Update(const void* data, uint32 length)
        {
                glBindBuffer(GL_UNIFORM_BUFFER, _bufferID);
                glBufferData(GL_UNIFORM_BUFFER, length, data, GL_STREAM_DRAW);
                glBindBufferBase(GL_UNIFORM_BUFFER, _binding_point, _bufferID);
        }

        THREAD_LOCAL_STORAGE uint8 UniformBuffer::_next_binding_point = 0;
        THREAD_LOCAL_STORAGE uint32 UniformBuffer::_max_binding_points = 0;

};