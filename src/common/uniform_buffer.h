#ifndef _UNIFORM_BUFFER_H
#define _UNIFORM_BUFFER_H

#include "config.h"

namespace ORC_NAMESPACE
{
        // GLSL layout std140 may require padding, read the specification
        class UniformBuffer final
        {

        public:

                UniformBuffer(uint32 programID, const string& name);
                ~UniformBuffer();

                template<typename block>
                void Update(const block& data)
                {
                        _Update(&data, sizeof(block));
                }


        private:

                void _Update(const void* data, uint32 length);

                uint32 _blockID;
                uint32 _bufferID;
                uint8 _binding_point;

                static THREAD_LOCAL_STORAGE uint8 _next_binding_point;
        };

};

#endif // _UNIFORM_BUFFER_H
