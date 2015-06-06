#include "skydome_shader.h"

#include "resource_loader.h"

#include "vertex_attributes.h"

namespace ORC_NAMESPACE
{

        SkydomeShader::SkydomeShader() :
                Shader(ResourceLoader::LoadShader("skydome.shader"), ResourceLoader::LoadShader("skydome.shader"), {{VERTEX_POSITION, "in_position"}, {VERTEX_UV, "in_uv"}})
        {}

        SkydomeShader::~SkydomeShader()
        {}

};