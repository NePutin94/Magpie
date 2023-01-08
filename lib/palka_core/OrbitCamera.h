#ifndef MAGPIE_ORBITCAMERA_H
#define MAGPIE_ORBITCAMERA_H

#include "EventManager.h"
#include <imgui.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <GLFW/glfw3.h>

namespace palka
{
    class OrbitCamera
    {
    public:
        OrbitCamera(const glm::vec3& center, const glm::vec3& upVector, float radius, float minRadius, float azimuthAngle, float polarAngle) :
                center_(center), upVector_(upVector), radius_(radius), _minRadius(minRadius), azimuthAngle_(azimuthAngle), polarAngle_(polarAngle)
        {
            bindEvents();
        }

        void rotateAzimuth(const float radians)
        {
            azimuthAngle_ += radians;

            // Keep azimuth angle within range <0..2PI) - it's not necessary, just to have it nicely output
            const auto fullCircle = 2.0f * glm::pi<float>();
            azimuthAngle_ = fmodf(azimuthAngle_, fullCircle);
            if(azimuthAngle_ < 0.0f)
            {
                azimuthAngle_ = fullCircle + azimuthAngle_;
            }
        }

        void rotatePolar(const float radians)
        {
            polarAngle_ += radians;

            // Check if the angle hasn't exceeded quarter of a circle to prevent flip, add a bit of epsilon like 0.001 radians
            const auto polarCap = glm::pi<float>() / 2.0f - 0.001f;
            if(polarAngle_ > polarCap)
            {
                polarAngle_ = polarCap;
            }

            if(polarAngle_ < -polarCap)
            {
                polarAngle_ = -polarCap;
            }
        }

        void zoom(const float distance)
        {
            radius_ += distance;
            if(radius_ < _minRadius)
            {
                radius_ = _minRadius;
            }
        }

        void moveHorizontal(const float distance)
        {
            const auto position = getEye();
            const glm::vec3 viewVector = getNormalizedViewVector();
            const glm::vec3 strafeVector = glm::normalize(glm::cross(viewVector, upVector_));
            center_ += strafeVector * distance;
        }

        void moveVertical(const float distance)
        {
            center_ += upVector_ * distance;
        }

        glm::mat4 getViewMatrix() const
        {
            const auto eye = getEye();
            return glm::lookAt(eye, eye + getNormalizedViewVector(), upVector_);
        }

        glm::vec3 getEye() const
        {
            // Calculate sines / cosines of angles
            const auto sineAzimuth = sin(azimuthAngle_);
            const auto cosineAzimuth = cos(azimuthAngle_);
            const auto sinePolar = sin(polarAngle_);
            const auto cosinePolar = cos(polarAngle_);

            // Calculate eye position out of them
            const auto x = center_.x + radius_ * cosinePolar * cosineAzimuth;
            const auto y = center_.y + radius_ * sinePolar;
            const auto z = center_.z + radius_ * cosinePolar * sineAzimuth;

            return glm::vec3(x, y, z);
        }

        glm::vec3 getViewPoint() const
        {
            return center_;
        }

        glm::vec3 getUpVector() const
        {
            return upVector_;
        }

        glm::vec3 getNormalizedViewVector() const
        {
            return glm::normalize(getViewPoint() - getEye());
        }

        float getAzimuthAngle() const
        {
            return azimuthAngle_;
        }

        float getPolarAngle() const
        {
            return polarAngle_;
        }

        float getRadius() const
        {
            return radius_;
        }

        void bindEvents()
        {
            idScroll = EventManager::addEvent(MouseEvent::WheelScrolled(), [this](EventData& data)
            {
                zoom(static_cast<float>(-data.MouseScroll.offsetY) * 0.5f);
            });

        }

        void unbindEvents()
        {
            EventManager::deleteMouseEvent(idScroll);
        }

        ~OrbitCamera()
        {
            unbindEvents();
        }

        void updateCamera(GLFWwindow* w)
        {
            const auto leftMouseButtonState = glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_LEFT);
            if(leftMouseButtonState == GLFW_PRESS)
            {
                if(!rotatingCamera && !movingCamera)
                {
                    rotatingCamera = true;
                    glfwGetCursorPos(w, &prevMousePosX, &prevMousePosY);
                }
            } else if(leftMouseButtonState == GLFW_RELEASE)
            {
                rotatingCamera = false;
            }

            // Check, if user started to move with orbit camera with middle mouse button
            const auto middleMouseButtonState = glfwGetMouseButton(w, GLFW_MOUSE_BUTTON_MIDDLE);
            if(middleMouseButtonState == GLFW_PRESS)
            {
                if(!rotatingCamera && !movingCamera)
                {
                    movingCamera = true;
                    glfwGetCursorPos(w, &prevMousePosX, &prevMousePosY);
                }
            } else if(middleMouseButtonState == GLFW_RELEASE)
            {
                movingCamera = false;
            }

            if(!(!rotatingCamera && !movingCamera))
            {
                // Only if we're rotating or moving we should calculate delta of mouse movement
                glfwGetCursorPos(w, &curMousePosX, &curMousePosY);
                const auto deltaX = static_cast<float>(curMousePosX - prevMousePosX);
                const auto deltaY = static_cast<float>(curMousePosY - prevMousePosY);

                if(rotatingCamera)
                {
                    rotateAzimuth(deltaX * 0.01f);
                    rotatePolar(deltaY * 0.01f);
                    prevMousePosX = curMousePosX;
                    prevMousePosY = curMousePosY;
                } else if(movingCamera)
                {
                    moveHorizontal(-deltaX * 0.05f);
                    moveVertical(deltaY * 0.05f);
                    prevMousePosX = curMousePosX;
                    prevMousePosY = curMousePosY;
                }
            }
        }

    private:
        glm::vec3 center_; // Center of the orbit camera sphere (the point upon which the camera looks)
        glm::vec3 upVector_; // Up vector of the camera
        float radius_; // Radius of the orbit camera sphere
        float _minRadius; // Minimal radius of the orbit camera sphere (cannot fall below this value)
        float azimuthAngle_; // Azimuth angle on the orbit camera sphere
        float polarAngle_; // Polar angle on the orbit camera sphere
        bool rotatingCamera = false;
        bool movingCamera = false;
        double prevMousePosX = 0.0;
        double prevMousePosY = 0.0;
        double curMousePosX = 0.0;
        double curMousePosY = 0.0;
        unsigned int idScroll;
    };
}
#endif //MAGPIE_ORBITCAMERA_H
