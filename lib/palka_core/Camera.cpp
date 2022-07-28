//
// Created by NePutin on 9/27/2021.
//

#include "Camera.h"
#ifdef REFLECTION_CORE

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<palka::Camera>("Camera")
            .constructor<>()
            .property("cameraPos", &palka::Camera::cameraPos)
            .property("cameraFront", &palka::Camera::cameraFront)
            .property("yaw", &palka::Camera::yaw)
            .property("pitch", &palka::Camera::pitch)
            .property("lastX", &palka::Camera::lastX)
            .property("lastY", &palka::Camera::lastY)
            .property("fov", &palka::Camera::fov)
            .property("size", &palka::Camera::size);
}

#endif