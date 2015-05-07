#ifndef _MESH_H
#define _MESH_H

#include "config.h"
#include "resource_types.h"

namespace ORC_NAMESPACE
{
        /*
        Mesh
        Loads the contents into vRAM using a vertex array object
        */
        class Mesh
        {

        public:

                Mesh(const MeshData& data);
                virtual ~Mesh();

                void Bind() const;
                void Draw() const;

                size_t Count() const;
                uint32 ID() const;
                static uint32 BoundID();

        protected:

                THREAD_LOCAL_STORAGE static uint32 _bound_VAO;

                const static uint8 _ATTRIBUTE_COUNT = 4;

                uint32 _VAO;
                uint32 _VBO[_ATTRIBUTE_COUNT];
                size_t _count;


        };

};

#endif // _MESH_H
