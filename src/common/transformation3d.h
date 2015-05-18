#ifndef _TRANSFORMATION_H
#define _TRASFORMATION_H

#include "config.h"

#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

namespace ORC_NAMESPACE
{
        /*
        Transformation3D
        Exposes convenience methods to perform standard transformations
        */
        class Transformation3D
        {
        public:

                Transformation3D();
                ~Transformation3D();

                void Translate(float dx, float dy, float dz);
                void Rotate(float rx, float ry, float rz);
                void Scale(float sx, float sy, float sz);

                void LoadIdentity();

                const float* GetModelMatrix() const;
                const float* GetNormalMatrix() const;

        private:
                glm::mat4 m_model;
                glm::mat4 m_normal;
        };
};

#endif // _TRANSFORMATION_H
