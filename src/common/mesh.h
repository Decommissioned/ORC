#ifndef _MESH_H
#define _MESH_H

#include "config.h"

namespace ORC_NAMESPACE
{

        class Mesh
        {

        public:

                Mesh(const vector<float>& positions, const vector<uint32> indices, const vector<float>& UVs);
                virtual ~Mesh();

                void Bind();
                void Draw() const;

        protected:

                uint32 _VAO;
                uint32 _VBO[16];
                uint32 _count;

        };

};

#endif // _MESH_H
