//
// Created by NePutin on 7/7/2021.
//

#include "Transform.h"

palka::Quad<float> palka::Transform::transformRect(palka::RectF rectangle) const
{
    const std::array<Vec2f, 4> points
            {
                    transformPoint(rectangle.left, rectangle.top),
                    transformPoint(rectangle.left, rectangle.top + rectangle.h),
                    transformPoint(rectangle.left + rectangle.w, rectangle.top),
                    transformPoint(rectangle.left + rectangle.w, rectangle.top + rectangle.h)
            };

    return Quad<float> {points};
}

palka::Vec2f palka::Transform::transformPoint(float x, float y) const
{
    return transformPoint({x, y});
}

palka::Vec2f palka::Transform::transformPoint(palka::Vec2f p) const
{
    return Vec2f(matrix[0] * p.x + matrix[4] * p.y + matrix[12],
                 matrix[1] * p.x + matrix[5] * p.y + matrix[13]);
}

palka::Transform palka::Transform::getInverse() const
{
    float det = matrix[0] * (matrix[15] * matrix[5] - matrix[7] * matrix[13]) -
                matrix[1] * (matrix[15] * matrix[4] - matrix[7] * matrix[12]) +
                matrix[3] * (matrix[13] * matrix[4] - matrix[5] * matrix[12]);
    if (det != 0.f)
    {
        return Transform((matrix[15] * matrix[5] - matrix[7] * matrix[13]) / det,
                         -(matrix[15] * matrix[4] - matrix[7] * matrix[12]) / det,
                         (matrix[13] * matrix[4] - matrix[5] * matrix[12]) / det,
                         -(matrix[15] * matrix[1] - matrix[3] * matrix[13]) / det,
                         (matrix[15] * matrix[0] - matrix[3] * matrix[12]) / det,
                         -(matrix[13] * matrix[0] - matrix[1] * matrix[12]) / det,
                         (matrix[7] * matrix[1] - matrix[3] * matrix[5]) / det,
                         -(matrix[7] * matrix[0] - matrix[3] * matrix[4]) / det,
                         (matrix[5] * matrix[0] - matrix[1] * matrix[4]) / det);
    } else
    {
        return Transform();
    }
}

palka::Transform palka::Transform::rotate(float angle, palka::Vec2f center)
{
    float rad = -angle * M_PI / 180.f;
    float cos = std::cos(rad);
    float sin = std::sin(rad);
    Transform rot(cos, -sin, center.x * (1 - cos) + center.y * sin,
                  sin, cos, center.y * (1 - cos) - center.x * sin,
                  0, 0, 1);
    return combine(rot);
}

palka::Transform palka::Transform::rotate(float angle)
{
    float rad = angle * M_PI / 180.f;
    float cos = std::cos(rad);
    float sin = std::sin(rad);
    Transform rot(cos, -sin, 0,
                  cos, sin,  0,
                  0, 0, 1);
    return combine(rot);
}

auto palka::Transform::scale(float x, float y, palka::Vec2f center)
{
    Transform scale(x, 0, center.x * (1 - center.x),
                    0, y, center.y * (1 - center.y),
                    0, 0, 1);
    return combine(scale);
}

palka::Transform palka::Transform::combine(const palka::Transform& tf)
{
    const float* a = matrix;
    const float* b = tf.matrix;
    auto _00 = a[0] * b[0] + a[4] * b[1] + a[12] * b[3];
    auto _01 = a[0] * b[4] + a[4] * b[5] + a[12] * b[7];
    auto _02 = a[0] * b[12] + a[4] * b[13] + a[12] * b[15];
    auto _10 = a[1] * b[0] + a[5] * b[1] + a[13] * b[3];
    auto _11 = a[1] * b[4] + a[5] * b[5] + a[13] * b[7];
    auto _12 = a[1] * b[12] + a[5] * b[13] + a[13] * b[15];
    auto _20 = a[3] * b[0] + a[7] * b[1] + a[15] * b[3];
    auto _21 = a[3] * b[4] + a[7] * b[5] + a[15] * b[7];
    auto _22 = a[3] * b[12] + a[7] * b[13] + a[15] * b[15];
    *this = Transform(_00, _01, _02,
                      _10, _11, _12,
                      _20, _21, _22);
    return *this;
}

auto palka::Transform::translate(float x, float y)
{
    Transform translate(1, 0, x,
                        0, 1, y,
                        0, 0, 1);
    return combine(translate);
}

auto palka::Transform::scale(float x, float y)
{
    Transform scale(x, 0, 0,
                    0, y, 0,
                    0, 0, 1);
    return combine(scale);
}

#ifdef REFLECTION_CORE
#include <rttr/registration>
RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<palka::Transform>("Transform")
            .constructor<palka::Transform>()
            .property("matrix", &palka::Transform::matrix);
}
#endif