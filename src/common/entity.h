#ifndef _ENTITY_H
#define _ENTITY_H

#include "config.h"

#include "transformation3d.h"

#include "mesh.h"
#include "generic_shader.h"
#include "texture.h"

namespace ORC_NAMESPACE
{

        /*
        Entity
        */
        class Entity
        {

        public:

                Entity(GenericShader& shader, const Mesh& mesh, const Texture2D& texture);
                virtual ~Entity();

                Transformation3D transform;

                void Render() const;

        protected:

                const Mesh& m_mesh;
                const Texture2D& m_texture;

                GenericShader& m_shader;

        };

};

#endif // _ENTITY_H
