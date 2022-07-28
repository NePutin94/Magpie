//
// Created by NePutin on 7/7/2021.
//

#ifndef PALKA_TRANSFORM_H
#define PALKA_TRANSFORM_H

#include "Vec2.h"
#include "Rect.h"
#include <array>

#ifdef REFLECTION_CORE
#include <rttr/type>
#include <rttr/registration_friend>
#endif

namespace palka
{
    class Transform
    {
#ifdef REFLECTION_CORE
        RTTR_ENABLE()
        RTTR_REGISTRATION_FRIEND
#endif
    private:
        float matrix[16];
    public:
        Transform()
        {
            matrix[0] = 1.f; matrix[4] = 0.f; matrix[8]  = 0.f; matrix[12] = 0.f;
            matrix[1] = 0.f; matrix[5] = 1.f; matrix[9]  = 0.f; matrix[13] = 0.f;
            matrix[2] = 0.f; matrix[6] = 0.f; matrix[10] = 1.f; matrix[14] = 0.f;
            matrix[3] = 0.f; matrix[7] = 0.f; matrix[11] = 0.f; matrix[15] = 1.f;
        }

        Transform(float a00, float a01, float a02,
                  float a10, float a11, float a12,
                  float a20, float a21, float a22)
        {
            matrix[0] = a00; matrix[4] = a01; matrix[8]  = 0.f; matrix[12] = a02;
            matrix[1] = a10; matrix[5] = a11; matrix[9]  = 0.f; matrix[13] = a12;
            matrix[2] = 0.f; matrix[6] = 0.f; matrix[10] = 1.f; matrix[14] = 0.f;
            matrix[3] = a20; matrix[7] = a21; matrix[11] = 0.f; matrix[15] = a22;
        }

        Transform combine(const Transform& tf);

        auto translate(float x, float y);

        auto scale(float x, float y);

        auto scale(float x, float y, Vec2f center);

        Transform rotate(float angle);

        Transform rotate(float angle, Vec2f center);

        auto& getMatrix()
        {
            return matrix;
        }

        [[nodiscard]] Transform getInverse() const;

        Vec2f transformPoint(Vec2f p) const;

        Vec2f transformPoint(float x, float y) const;

        Quad<float> transformRect(RectF rectangle) const;
    };
}

#endif //PALKA_TRANSFORM_H
