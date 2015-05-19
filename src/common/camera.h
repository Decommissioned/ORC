#ifndef _CAMERA_H
#define _CAMERA_H

#include "config.h"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace ORC_NAMESPACE
{

        class Camera
        {

        public:

                Camera();
                ~Camera();

                glm::mat4 View() const;
                glm::vec3 Position() const;

                float Roll() const;
                float Pitch() const;

                void Roll(float theta);
                void Pitch(float theta);

                void Set(const glm::vec3& lookat);
                void Move(const glm::vec3& move);
                void Distance(float distance);
                void IncDistance(float delta);

        protected:

                float m_roll;
                float m_pitch;

                glm::vec3 m_lookat;
                glm::vec3 m_direction;

                float m_distance;

        };

};

#endif // _CAMERA_H
