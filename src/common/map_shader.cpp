#include "map_shader.h"

#include "vertex_attributes.h"
#include "resource_loader.h"

namespace ORC_NAMESPACE
{

        MapShader::MapShader() :
                Shader(
                ResourceLoader::LoadShader("map_vs.shader"),
                ResourceLoader::LoadShader("map_fs.shader"),
                {{VERTEX_POSITION, "position"}})
        {

        }

        MapShader::~MapShader()
        {

        }

};