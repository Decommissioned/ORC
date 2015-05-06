#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "config.h"

namespace ORC_NAMESPACE
{

        enum class TextureWrapping
        {
                REPEAT,
                MIRRORED_REPEAT,
                CLAMP_TO_EDGE,
                CLAMP_TO_BORDER
        };

        enum class TextureFiltering
        {
                NEAREST,
                LINEAR,
                NEAREST_MIPMAP,
                LINEAR_MIPMAP
        };

        enum class TextureChannel
        {
                GRAYSCALE,
                RGBA
        };

        /*
        Texture2D
        TODO: accept raw pixel data instead of actually loading the image
        */
        class Texture2D final
        {
        public:

                explicit Texture2D(const string& path, TextureWrapping wrapping, TextureFiltering filtering, TextureChannel channel);
                explicit Texture2D(const string& path, TextureFiltering filtering);
                explicit Texture2D(const string& path, TextureWrapping wrapping);
                explicit Texture2D(const string& path);

                Texture2D(Texture2D&) = delete;
                ~Texture2D();

                void Bind() const;
                uint32 ID() const;
                static uint32 BoundID();

        private:

                uint32 _wrapping_mode;
                uint32 _filtering_mode;
                uint32 _textureID;

                THREAD_LOCAL_STORAGE static uint32 _bound_textureID;
        };

};

#endif // _TEXTURE_H
