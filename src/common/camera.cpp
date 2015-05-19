#include "camera.h"

#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

namespace ORC_NAMESPACE
{
        const glm::vec3 i = glm::vec3(1.0f, 0.0f, 0.0f);
        const glm::vec3 j = glm::vec3(0.0f, 1.0f, 0.0f);
        const glm::vec3 k = glm::vec3(0.0f, 0.0f, 1.0f);

        const glm::vec3 up = {0.0f, 1.0f, 0.0f};

        glm::mat4 Camera::View() const
        {
                glm::vec3 eye = (m_lookat - glm::rotate(m_direction, m_roll, up) * m_distance);
                eye.y = tanf(m_pitch);
                
                return glm::lookAt(eye, m_lookat, up);
        }

        glm::vec3 Camera::Position() const
        {
                return m_lookat - glm::rotate(m_direction, m_roll, up);
        }

        void Camera::Set(const glm::vec3& lookat)
        {
                m_lookat = lookat;
        }

        void Camera::Move(const glm::vec3& move)
        {
                glm::vec3 actual_movement = glm::rotate(move, m_roll, up);
                m_lookat += actual_movement;
        }        

        void Camera::Distance(float distance)
        {
                m_distance = distance;
        }

        void Camera::IncDistance(float delta)
        {
                m_distance += delta;
                if (std::fabsf(m_distance) < glm::epsilon<float>()) m_distance += delta;
        }

        Camera::Camera()
        {
                m_lookat = {0.0f, 0.0f, 0.0f};
                m_direction = {0.0f, 0.0f, 1.0f};
                m_distance = 1.0f;
                m_roll = 0;
        }

        Camera::~Camera()
        {}

        void Camera::Roll(float theta)
        {
                m_roll += theta;
        }

        float Camera::Roll() const
        {
                return m_roll;
        }

        void Camera::Pitch(float theta)
        {
                m_pitch += theta;
                m_pitch = glm::clamp(m_pitch, -glm::quarter_pi<float>(), glm::quarter_pi<float>());
        }

        float Camera::Pitch() const
        {
                return m_pitch;
        }

};