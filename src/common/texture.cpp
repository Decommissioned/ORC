#include "texture.h"
#include "resource_loader.h"

#include <GL/glew.h>

namespace ORC_NAMESPACE
{

        Texture2D::Texture2D(const ImageData& img, TextureWrapping wrapping, TextureFiltering filtering, TextureChannel channel)
        {
                // TODO: fix mip map generation 
                m_nameID = img.nameID;

                glGenTextures(1, &m_textureID);

                Bind();

                if (channel == TextureChannel::RGBA)
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img.data.front());
                else
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img.data.front());

                switch (wrapping)
                {
                case orc::TextureWrapping::REPEAT:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                        break;
                case orc::TextureWrapping::MIRRORED_REPEAT:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
                        break;
                case orc::TextureWrapping::CLAMP_TO_EDGE:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                        break;
                case orc::TextureWrapping::CLAMP_TO_BORDER:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
                        break;
                }

                switch (filtering)
                {
                case orc::TextureFiltering::NEAREST_MIPMAP:
                        glGenerateMipmap(GL_TEXTURE_2D);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                        break;
                case orc::TextureFiltering::NEAREST:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                        break;
                case orc::TextureFiltering::LINEAR_MIPMAP:
                        glGenerateMipmap(GL_TEXTURE_2D);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                        break;
                case orc::TextureFiltering::LINEAR:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        break;
                }
        }

        Texture2D::Texture2D(const ImageData& img, TextureFiltering filtering)
                : Texture2D(img, TextureWrapping::REPEAT, TextureFiltering::LINEAR, TextureChannel::RGBA)
        {}

        Texture2D::Texture2D(const ImageData& img, TextureWrapping wrapping)
                : Texture2D(img, wrapping, TextureFiltering::LINEAR, TextureChannel::RGBA)
        {}

        Texture2D::Texture2D(const ImageData& img)
                : Texture2D(img, TextureWrapping::REPEAT, TextureFiltering::LINEAR, TextureChannel::RGBA)
        {}

        Texture2D::~Texture2D()
        {
                glDeleteTextures(1, &m_textureID);
        }

        void Texture2D::Bind() const
        {
                glBindTexture(GL_TEXTURE_2D, m_textureID);
                m_bound_textureID = m_textureID;
        }

        uint32 Texture2D::ID() const
        {
                return m_textureID;
        }

        uint32 Texture2D::BoundID()
        {
                return m_bound_textureID;
        }

        const string& Texture2D::NameID() const
        {
                return m_nameID;
        }

        THREAD_LOCAL_STORAGE uint32 Texture2D::m_bound_textureID = 0;

};