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

                UniformBuffer(std::initializer_list<uint32> programIDs, const string& name);
                ~UniformBuffer();

                template<typename block>
                void Update(const block& data)
                {
                        _Update(&data, sizeof(block));
                }


        private:

                void _Update(const void* data, uint32 length);

                uint32 _bufferID;
                uint8 _binding_point;

                static THREAD_LOCAL_STORAGE uint8 _next_binding_point;
                static THREAD_LOCAL_STORAGE uint32 _max_binding_points;
        };

};

#endif // _UNIFORM_BUFFER_H
