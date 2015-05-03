#include "uniform_buffer.h"

#include <GL/glew.h>
#include <mutex>

namespace ORC_NAMESPACE
{
        std::mutex _M;

        UniformBuffer::UniformBuffer(const Shader& program, const string& name)
        {
                _binding_point = _next_binding_point;
                _next_binding_point++;

                uint32 programID = program.ID();
                _blockID = glGetUniformBlockIndex(programID, name.c_str());
                glUniformBlockBinding(programID, _blockID, _binding_point);

                glGenBuffers(1, &_bufferID);
        }

        UniformBuffer::~UniformBuffer()
        {
                glDeleteBuffers(1, &_bufferID);
        }

        void UniformBuffer::Update(const void* data, uint32 length)
        {
                glBindBuffer(GL_UNIFORM_BUFFER, _bufferID);
                glBufferData(GL_UNIFORM_BUFFER, length, data, GL_DYNAMIC_DRAW);
                glBindBufferBase(GL_UNIFORM_BUFFER, _binding_point, _bufferID);
        }

        THREAD_LOCAL_STORAGE uint8 UniformBuffer::_next_binding_point = 0;

};