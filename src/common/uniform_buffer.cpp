#include "uniform_buffer.h"
#include "error_codes.h"

#include <GL/glew.h>

namespace ORC_NAMESPACE
{

        UniformBuffer::UniformBuffer()
        {       
                if (m_max_binding_points == 0) // Thread "singleton" variable
                {
                        int32 max;
                        glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &max);

                        if (max <= 0) throw Error::OPENGL_UNIFORM_BUFFER_QUERY;

                        m_max_binding_points = max;
                }

                if (m_next_binding_point >= m_max_binding_points)
                        throw Error::OPENGL_UNIFORM_BUFFER_FULL;

                m_binding_point = m_next_binding_point;
                m_next_binding_point++;

                glGenBuffers(1, &m_bufferID);
        }

        UniformBuffer::~UniformBuffer()
        {
                glDeleteBuffers(1, &m_bufferID);
        }

        void UniformBuffer::update(const void* data, uint32 length)
        {
                glBindBuffer(GL_UNIFORM_BUFFER, m_bufferID);
                glBufferData(GL_UNIFORM_BUFFER, length, data, GL_STREAM_DRAW);
                glBindBufferBase(GL_UNIFORM_BUFFER, m_binding_point, m_bufferID);
        }

        void UniformBuffer::AddPrograms(std::initializer_list<uint32> programIDs, const string& name)
        {
                for (auto programID : programIDs)
                {
                        uint32 blockID = glGetUniformBlockIndex(programID, name.c_str());
                        glUniformBlockBinding(programID, blockID, m_binding_point);
                }
        }

        THREAD_LOCAL_STORAGE uint8 UniformBuffer::m_next_binding_point = 0;
        THREAD_LOCAL_STORAGE uint32 UniformBuffer::m_max_binding_points = 0;

};