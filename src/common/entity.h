#ifndef _ENTITY_H
#define _ENTITY_H

#include "config.h"

#include "transformation3d.h"

namespace ORC_NAMESPACE
{

        class Entity
        {

        public:

                Entity(uint32 mesh, uint32 vertices, uint32 program, uint32 texture);
                virtual ~Entity();

                Transformation3D transform;

                void Render();

        protected:

                uint32 _mesh;
                uint32 _vertices;
                uint32 _program;
                uint32 _texture;

        };

};

#endif // _ENTITY_H
