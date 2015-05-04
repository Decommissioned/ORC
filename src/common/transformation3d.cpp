#include "transformation3d.h"

#include <memory>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace ORC_NAMESPACE
{
        const static glm::vec3 i = {1.0f, 0.0f, 0.0f};
        const static glm::vec3 j = {0.0f, 1.0f, 0.0f};
        const static glm::vec3 k = {0.0f, 0.0f, 1.0f};

        Transformation3D::Transformation3D()
        {}
        Transformation3D::~Transformation3D()
        {}

        void Transformation3D::Translate(float dx, float dy, float dz)
        {
                _model = glm::translate(_model, glm::vec3(dx, dy, dz));
        }

        void Transformation3D::Rotate(float rx, float ry, float rz)
        {
                // OPTIMIZE: use quaternions in the future if possible
                _model = glm::rotate(glm::rotate(glm::rotate(_model, rx, i), ry, j), rz, k);
                _normal = glm::rotate(glm::rotate(glm::rotate(_normal, rx, i), ry, j), rz, k);
        }

        void Transformation3D::LoadIdentity()
        {
                _model = _normal = glm::mat4();
        }

        void Transformation3D::Scale(float sx, float sy, float sz)
        {
                _model = glm::scale(_model, glm::vec3(sx, sy, sz));
        }

        const float* Transformation3D::GetModelMatrix() const
        {
                return &_model[0][0];
        }

        const float* Transformation3D::GetNormalMatrix() const
        {
                return &_normal[0][0];
        }

};