#include "skydome.h"

#include "resource_loader.h"

namespace ORC_NAMESPACE
{

        Skydome::Skydome(const Texture2D& texture) :
                _dome_shader(SkydomeShader()),
                _dome(ResourceLoader::LoadOBJ("skydome.obj")),
                _texture(texture)
        {}

        Skydome::~Skydome()
        {}

        void Skydome::BindIfNeeded() const
        {
                if (Shader::BoundID() != _dome_shader.ID())
                        _dome_shader.Bind();

                if (Mesh::BoundID() != _dome.ID())
                        _dome.Bind();

                if (Texture2D::BoundID() != _texture.ID())
                        _texture.Bind();
        }

        void Skydome::Draw() const
        {
                BindIfNeeded();
                _dome.Draw();
        }

        uint32 Skydome::ShaderID() const
        {
                return _dome_shader.ID();
        }

};