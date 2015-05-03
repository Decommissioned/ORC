#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "config.h"

namespace ORC_NAMESPACE
{

        enum class TextureWrapping : uint32
        {
                REPEAT,
                MIRRORED_REPEAT,
                CLAMP_TO_EDGE,
                CLAMP_TO_BORDER
        };

        enum class TextureFiltering : uint32
        {
                NEAREST,
                LINEAR,
                NEAREST_MIPMAP,
                LINEAR_MIPMAP
        };

        class Texture2D final
        {
        public:

                explicit Texture2D(const string& path, TextureWrapping wrapping, TextureFiltering filtering);
                explicit Texture2D(const string& path, TextureFiltering filtering);
                explicit Texture2D(const string& path, TextureWrapping wrapping);
                explicit Texture2D(const string& path);

                Texture2D(Texture2D&) = delete;
                ~Texture2D();

                void Bind();
                uint32 ID() const;

        private:

                uint32 _wrapping_mode;
                uint32 _filtering_mode;
                uint32 _textureID;
        };

};

#endif // _TEXTURE_H
