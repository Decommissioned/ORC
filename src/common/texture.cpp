#include "texture.h"
#include "resource_loader.h"

#include <GL/glew.h>

namespace ORC_NAMESPACE
{

        Texture::Texture(const string& path, TextureWrapping wrapping, TextureFiltering filtering)
        {
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

        Texture::Texture(const string& path, TextureFiltering filtering)
                : Texture(path, TextureWrapping::REPEAT, TextureFiltering::LINEAR)
        {}

        Texture::Texture(const string& path, TextureWrapping wrapping)
                : Texture(path, wrapping, TextureFiltering::LINEAR)
        {}

        Texture::Texture(const string& path)
                : Texture(path, TextureWrapping::REPEAT, TextureFiltering::LINEAR)
        {}

        Texture::~Texture()
        {
                glDeleteTextures(1, &_textureID);
        }

        void Texture::Bind()
        {
                glBindTexture(GL_TEXTURE_2D, _textureID);
        }

};