#include "texture.h"
#include "resource_loader.h"

#include <GL/glew.h>

namespace ORC_NAMESPACE
{

        Texture2D::Texture2D(const string& path, TextureWrapping wrapping, TextureFiltering filtering)
        {
                // TODO: fix mip map generation 
                Image img = ResourceLoader::LoadImage(path);

                glGenTextures(1, &_textureID);

                glBindTexture(GL_TEXTURE_2D, _textureID);

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
                case orc::TextureFiltering::NEAREST:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                        break;
                case orc::TextureFiltering::LINEAR_MIPMAP:
                        glGenerateMipmap(GL_TEXTURE_2D);
                case orc::TextureFiltering::LINEAR:
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                        break;
                }

                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &img.data[0]);
        }

        Texture2D::Texture2D(const string& path, TextureFiltering filtering)
                : Texture2D(path, TextureWrapping::REPEAT, TextureFiltering::LINEAR)
        {}

        Texture2D::Texture2D(const string& path, TextureWrapping wrapping)
                : Texture2D(path, wrapping, TextureFiltering::LINEAR)
        {}

        Texture2D::Texture2D(const string& path)
                : Texture2D(path, TextureWrapping::REPEAT, TextureFiltering::LINEAR)
        {}

        Texture2D::~Texture2D()
        {
                glDeleteTextures(1, &_textureID);
        }

        void Texture2D::Bind()
        {
                glBindTexture(GL_TEXTURE_2D, _textureID);
        }

        uint32 Texture2D::ID() const
        {
                return _textureID;
        }

};