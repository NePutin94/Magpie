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
//    template<class T>
//    using Vec2 = glm::vec<2, T, glm::defaultp>;
    template<class T>
    struct Vec2
    {
        T x;
        T y;

        Vec2() = default;

        Vec2(T x, T y) : x(x), y(y)
        {}

        Vec2 operator+(const Vec2& other) const
        {
            return {x + other.x, y + other.y};
        }

        Vec2& operator+=(const Vec2& other)
        {
            *this = *this + other;
            return *this;
        }

        Vec2 operator-(const Vec2& other) const
        {
            return {x - other.x, y - other.y};
        }

        Vec2& operator-=(const Vec2& other)
        {
            *this = *this - other;
            return *this;
        }

        Vec2 operator*(const Vec2& other) const
        {
            return {x * other.x, y * other.y};
        }

        Vec2& operator*=(const Vec2& other)
        {
            *this = *this * other;
            return *this;
        }

        Vec2 operator-() const
        {
            return {-x, -y};
        }

        Vec2 operator/(const Vec2& other) const
        {
            return {x / other.x, y / other.y};
        }

        Vec2& operator/=(const Vec2& other)
        {
            *this = *this / other;
            return *this;
        }

        bool operator==(const Vec2& other) const
        {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Vec2& other) const
        {
            return x != other.x && y != other.y;
        }

        auto convert() const
        {
            return glm::vec<2, T, glm::defaultp>(x, y);
        }

        static auto convertFrom(glm::vec<2, T, glm::defaultp> vec)
        {
            return Vec2(vec.x, vec.y);
        }
    };

    using Vec2f = glm::vec2;
    using Vec3f = glm::vec3;
    using Vec3i = glm::ivec3;
    using Mat2f = glm::mat2;
    using Mat3f = glm::mat3;
    using Mat4f = glm::mat4;
    using Vec2i = glm::ivec2;
}
namespace glm
{

    template<class T>
    auto dot(const palka::Vec2 <T>& vec1, const palka::Vec2 <T>& vec2)
    {
        return glm::dot(vec1.convert(), vec2.convert());
    }

    template<class T>
    auto normalize(const palka::Vec2 <T>& vec)
    {
        return palka::Vec2<T>::convertFrom(glm::normalize(vec.convert()));
    }
}
#endif //PALKA_VEC2_H
