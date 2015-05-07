#include "skydome_shader.h"

#include "resource_loader.h"

namespace ORC_NAMESPACE
{

        SkydomeShader::SkydomeShader() :
                Shader(ResourceLoader::LoadShader("skydome.shader"), ResourceLoader::LoadShader("skydome.shader"))
        {

        }

        SkydomeShader::~SkydomeShader()
        {}

};