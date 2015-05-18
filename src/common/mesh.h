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

                const string& NameID() const;

        protected:

                THREAD_LOCAL_STORAGE static uint32 m_bound_VAO;

                const static uint8 m_ATTRIBUTE_COUNT = 4;

                uint32 m_VAO;
                uint32 m_VBO[m_ATTRIBUTE_COUNT];
                size_t m_count;

                string m_nameID;
        };

};

#endif // _MESH_H
