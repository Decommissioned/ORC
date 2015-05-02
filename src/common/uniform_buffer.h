#ifndef _UNIFORM_BUFFER_H
#define _UNIFORM_BUFFER_H

#include "shader.h"

namespace ORC_NAMESPACE
{

        class UniformBuffer final
        {

        public:

                UniformBuffer(const Shader& program, const string& name);
                ~UniformBuffer();

                template<typename block>
                void Update(const block& data)
                {
                        Update(&data, sizeof(block));
                }


        private:

                void Update(const void* data, uint32 length);

                uint32 _blockID;
                uint32 _bufferID;
                uint8 _binding_point;

                static THREAD_LOCAL_STORAGE uint8 _next_binding_point;
        };

};

#endif // _UNIFORM_BUFFER_H
