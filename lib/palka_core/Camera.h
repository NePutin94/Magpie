//
// Created by NePutin on 9/27/2021.
//

#ifndef PALKA_CAMERA_H
#define PALKA_CAMERA_H

#include "EventManager.h"
#include <imgui.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "Vec2.h"

namespace palka
{
    class Camera
    {
    public:
        Camera() : up(0, 1, 0), rotation(0), fov(45)
        {
            bind();
        }

        Camera(Vec2f sz) : up(0, 1, 0), rotation(0), fov(45), size(sz)
        {
            bind();
        }

        Vec3f cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        Vec3f cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
        Vec3f cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 direction;
        bool firstMouse = true;
        float yaw = -90;
        float pitch = 0;
        float lastX, lastY;
        Vec2f mousePos;
        bool keyPress = false;
        float sensitivity = 0.1f;

        glm::mat4 getViewMatrix()
        {
            return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        }

        glm::mat4 getProjectionMatrix()
        {
            return glm::perspective(glm::radians(fov), (float) size.x / (float) size.y, 0.1f, 100.0f);
        }

        std::set<unsigned int> inputIds;
        std::set<unsigned int> mouseEventIds;

        void bind()
        {
            inputIds.emplace(EventManager::addInput(GLFW_KEY_W, [this]()
            {
                cameraPos += cameraSpeed * cameraFront;
            }));
            inputIds.emplace(EventManager::addInput(GLFW_KEY_SPACE, [this]()
            {
                cameraPos.y += cameraSpeed;
            }));
            inputIds.emplace(EventManager::addInput(GLFW_KEY_LEFT_CONTROL, [this]()
            {
                cameraPos.y -= cameraSpeed;
            }));
            inputIds.emplace(EventManager::addInput(GLFW_KEY_S, [this]()
            {
                cameraPos -= cameraSpeed * cameraFront;
            }));
            inputIds.emplace(EventManager::addInput(GLFW_KEY_A, [this]()
            {
                cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            }));
            inputIds.emplace(EventManager::addInput(GLFW_KEY_D, [this]()
            {
                cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
            }));
            mouseEventIds.emplace(EventManager::addEvent(MouseEvent::ButtonPressed(MouseEvent::Mouse_Button::Left), [this](EventData& data)
            {
                if(!ImGui::IsAnyItemHovered() && !ImGui::IsAnyItemFocused())
                {
                    keyPress = true;
                    firstMouse = true;
                }
            }));
            mouseEventIds.emplace(EventManager::addEvent(MouseEvent::ButtonReleased(MouseEvent::Mouse_Button::Left), [this](EventData& data)
            {
                keyPress = false;
            }));
            mouseEventIds.emplace(EventManager::addEvent(MouseEvent::Motion(), [this](EventData& data)
            {
                if(keyPress)
                {
                    if(firstMouse) // initially set to true
                    {
                        lastX = data.MouseMotion.x;
                        lastY = data.MouseMotion.y;
                        firstMouse = false;
                    }

                    float xoffset = data.MouseMotion.x - lastX;
                    float yoffset = lastY - data.MouseMotion.y; // reversed since y-coordinates range from bottom to top
                    lastX = data.MouseMotion.x;
                    lastY = data.MouseMotion.y;

                    xoffset *= sensitivity;
                    yoffset *= sensitivity;
                    yaw += xoffset;
                    pitch += yoffset;
                    if(pitch > 89.0f)
                        pitch = 89.0f;
                    if(pitch < -89.0f)
                        pitch = -89.0f;
                    glm::vec3 direction;
                    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                    direction.y = sin(glm::radians(pitch));
                    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
                    cameraFront = glm::normalize(direction);
                }
            }));
        }

        void unbind()
        {
            for(auto& id: inputIds)
                EventManager::deleteKBoasrdInput(id);
            for(auto& id: mouseEventIds)
                EventManager::deleteMouseEvent(id);
        }

        ~Camera()
        {
            unbind();
        }

    public:
        //View
        float cameraSpeed = 0.005f;
        Vec3f up;
        Vec3f viewP;
        float rotation;
        //Projection
        float fov;
        Vec2f size;


        glm::mat4 view;
        glm::mat4 proj;
    };
}

#endif //PALKA_CAMERA_H
