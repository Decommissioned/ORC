#include "texture.h"
#include "resource_loader.h"

#include <GL/glew.h>

namespace ORC_NAMESPACE
{

        Texture2D::Texture2D(const string& path, TextureWrapping wrapping, TextureFiltering filtering, TextureChannel channel)
        {
                // TODO: fix mip map generation 
                ImageData img = ResourceLoader::LoadPNG(path);

                glGenTextures(1, &_textureID);

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

        Texture2D::Texture2D(const string& path, TextureFiltering filtering)
                : Texture2D(path, TextureWrapping::REPEAT, TextureFiltering::LINEAR, TextureChannel::RGBA)
        {}

        Texture2D::Texture2D(const string& path, TextureWrapping wrapping)
                : Texture2D(path, wrapping, TextureFiltering::LINEAR, TextureChannel::RGBA)
        {}

        Texture2D::Texture2D(const string& path)
                : Texture2D(path, TextureWrapping::REPEAT, TextureFiltering::LINEAR, TextureChannel::RGBA)
        {}

        Texture2D::~Texture2D()
        {
                glDeleteTextures(1, &_textureID);
        }

        void Texture2D::Bind() const
        {
                glBindTexture(GL_TEXTURE_2D, _textureID);
                _bound_textureID = _textureID;
        }

        uint32 Texture2D::ID() const
        {
                return _textureID;
        }

        uint32 Texture2D::BoundID()
        {
                return _bound_textureID;
        }

        THREAD_LOCAL_STORAGE uint32 Texture2D::_bound_textureID = 0;

};