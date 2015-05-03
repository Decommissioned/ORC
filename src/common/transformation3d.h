#ifndef _TRANSFORMATION_H
#define _TRASFORMATION_H

#include "config.h"

#include <glm/mat4x4.hpp>

namespace ORC_NAMESPACE
{
        class Transformation3D
        {
        public:

                Transformation3D();
                ~Transformation3D();

                void Translate(float dx, float dy, float dz);
                void Rotate(float rx, float ry, float rz);
                void Scale(float sx, float sy, float sz);

                void LoadIdentity();

                const float* GetMatrixPointer() const;

        private:
                glm::mat4 data;
        };
};

#endif // _TRANSFORMATION_H
