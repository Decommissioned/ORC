#ifndef _TEXTURE_H
#define _TEXTURE_H

#include "config.h"

#include "resource_types.h"

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

                explicit Texture2D(const ImageData& img, TextureWrapping wrapping, TextureFiltering filtering, TextureChannel channel);
                explicit Texture2D(const ImageData& img, TextureFiltering filtering);
                explicit Texture2D(const ImageData& img, TextureWrapping wrapping);
                explicit Texture2D(const ImageData& img);

                Texture2D(Texture2D&) = delete;
                ~Texture2D();

                void Bind() const;
                uint32 ID() const;
                static uint32 BoundID();

                const string& NameID() const;

        private:

                uint32 m_wrapping_mode;
                uint32 m_filtering_mode;
                uint32 m_textureID;

                THREAD_LOCAL_STORAGE static uint32 m_bound_textureID;

                string m_nameID;
        };

};

#endif // _TEXTURE_H
