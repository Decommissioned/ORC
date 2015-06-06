#include "map_shader.h"

#include "vertex_attributes.h"
#include "resource_loader.h"

#include <GL/glew.h>

namespace ORC_NAMESPACE
{

        MapShader::MapShader() :
                Shader(
                ResourceLoader::LoadShader("map_vs.shader"),
                ResourceLoader::LoadShader("map_fs.shader"),
                {{VERTEX_POSITION, "in_position"}, {VERTEX_NORMAL, "in_normal"}})
        {
                m_loc_sun = glGetUniformLocation(m_programID, "sun");
        }

        MapShader::~MapShader()
        {

        }

        void MapShader::SetSun(const glm::vec3& direction)
        {
                glUniform3f(m_loc_sun, direction.x, direction.y, direction.z);
        }

};