//
// Created by NePutin on 6/23/2021.
//

#ifndef PALKA_VEC2_H
#define PALKA_VEC2_H

#include "config.h"
#include <glm/vec2.hpp>
#include <glm/detail/qualifier.hpp>
#include <glm/vec3.hpp>
#include <glm/mat2x2.hpp>
#include <cmath>
#include <string>

#ifdef REFLECTION_CORE

#include <rttr/type>
#include <rttr/registration_friend>

#endif
namespace palka
{
    template<class T>
    using Vec2 = glm::vec<2, T, glm::defaultp>;
    using Vec2f = glm::vec2;
    using Vec3f = glm::vec3;
    using Vec3i = glm::ivec3;
    using Mat2f = glm::mat2;
    using Mat3f = glm::mat3;
    using Mat4f = glm::mat4;
    using Vec2i = glm::ivec2;
}
#endif //PALKA_VEC2_H
