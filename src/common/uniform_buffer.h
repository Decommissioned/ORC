#ifndef _UNIFORM_BUFFER_H
#define _UNIFORM_BUFFER_H

#include "config.h"

namespace ORC_NAMESPACE
{
        /*
        Uniform Buffer
        Allows a buffer of data to be used in multiple shaders in one go
        Requires std140 to function, if not familiar with it you may wish to search for it in the OpenGL spec.
        */
        class UniformBuffer final
        {

        public:

                UniformBuffer();

                ~UniformBuffer();

                void AddPrograms(std::initializer_list<uint32> programIDs, const string& name);

                template<typename block>
                void Update(const block& data)
                {
                        update(&data, sizeof(block));
                }


        private:

                void update(const void* data, uint32 length);

                uint32 m_bufferID;
                uint8 m_binding_point;

                static THREAD_LOCAL_STORAGE uint8 m_next_binding_point;
                static THREAD_LOCAL_STORAGE uint32 m_max_binding_points;
        };

};

#endif // _UNIFORM_BUFFER_H
