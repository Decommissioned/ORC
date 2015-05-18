#ifndef _SKYDOME_H
#define _SKYDOME_H

#include "skydome_shader.h"
#include "entity.h"

namespace ORC_NAMESPACE
{

        class Skydome final
        {
        public:

                Skydome(const Texture2D& texture);

                Skydome(const Skydome&) = delete;
                ~Skydome();

                void Draw() const;
                uint32 ShaderID() const;

        private:

                void BindIfNeeded() const;

                const Texture2D& m_texture;
                SkydomeShader m_dome_shader;
                Mesh m_dome;

        };

};

#endif // _SKYDOME_H
