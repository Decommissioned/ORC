#include "transformation3d.h"

#include <memory>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

namespace ORC_NAMESPACE
{
        const static glm::mat4 identity;
        const static glm::vec3 i = {1.0f, 0.0f, 0.0f};
        const static glm::vec3 j = {0.0f, 1.0f, 0.0f};
        const static glm::vec3 k = {0.0f, 0.0f, 1.0f};

        Transformation3D::Transformation3D() {}
        Transformation3D::~Transformation3D() {}

        void Transformation3D::Translate(float dx, float dy, float dz)
        {
                data *= glm::translate(identity, glm::vec3(dx, dy, dz));
        }

        void Transformation3D::Rotate(float rx, float ry, float rz)
        {
                // OPTIMIZE: use quaternions in the future if possible
                data *=
                glm::rotate(identity, rx, i) *
                glm::rotate(identity, ry, j) *
                glm::rotate(identity, rz, k);
        }

        void Transformation3D::Scale(float sx, float sy, float sz)
        {
                data *= glm::scale(identity, glm::vec3(sx, sy, sz));
        }

        const float* Transformation3D::GetMatrixPointer() const
        {
                return &data[0][0];
        }

        void Transformation3D::LoadIdentity()
        {
                data = identity;
        }

};