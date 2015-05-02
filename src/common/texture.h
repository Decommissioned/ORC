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

        class Texture final
        {
        public:

                explicit Texture(const string& path, TextureWrapping wrapping, TextureFiltering filtering);
                explicit Texture(const string& path, TextureFiltering filtering);
                explicit Texture(const string& path, TextureWrapping wrapping);
                explicit Texture(const string& path);

                Texture(Texture&) = delete;
                ~Texture();

                void Bind();

        private:

                uint32 _wrapping_mode;
                uint32 _filtering_mode;
                uint32 _textureID;
        };

};

#endif // _TEXTURE_H
