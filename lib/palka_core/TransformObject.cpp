//
// Created by NePutin on 8/2/2021.
//

#include "TransformObject.h"

palka::Transform palka::TransformObject::getTransform() const
{
    if (updateTransform)
    {
        float rot = angle * M_PI / 180.f;
        float cosine = std::cos(rot);
        float sine = std::sin(rot);
        float cx = scale.x * cosine;
        float cy = scale.y * cosine;
        float sx = scale.x * sine;
        float sy = scale.y * sine;
        Vec2f center2 = center * scale;
        float tx = center2.x * (1 - cosine) + center2.y * sine + pos.x;
        float ty = center2.y * (1 - cosine) - center2.x * sine + pos.y;

        t = Transform(cx, -sy, tx,
                      sx, cy, ty,
                      0.f, 0.f, 1.f);
        updateTransform = false;
    }
    return t;
}

palka::Quad<float> palka::TransformObject::getQuad(palka::RectF rect)
{
    auto points = rect.getPoints();
    getTransform();
    auto test = t.transformRect(rect);
    //auto test2 = t.transformRect({rect.left, rect.h, rect.w, rect.top});
//            Quad<float>({Vec2{test.left, test.top},
//                         Vec2{test.w, test.h},
//                         Vec2{test2.left, test2.top},
//                         Vec2{test2.w, test2.h}})
    return test;
}

palka::Vec2f palka::TransformObject::getCenter() const
{
    return center;
}

void palka::TransformObject::setCenter(palka::Vec2f center)
{
    this->center = center;
    updateTransform = true;
}

float palka::TransformObject::getRotation() const
{
    return angle;
}

void palka::TransformObject::setRotation(float angle)
{
    this->angle = angle;
    updateTransform = true;
}

palka::Vec2f palka::TransformObject::getScale() const
{
    return scale;
}

void palka::TransformObject::setScale(palka::Vec2f scale)
{
    this->scale = scale;
    updateTransform = true;
}

palka::Vec2f palka::TransformObject::getPosition() const
{
    return pos;
}

void palka::TransformObject::setPosition(palka::Vec2f pos)
{
    this->pos = pos;
    updateTransform = true;
}

#ifdef REFLECTION_CORE
#include <rttr/registration>
RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<palka::TransformObject>("TransformObject")
            .constructor<>()
            .property("position", &palka::TransformObject::getPosition, &palka::TransformObject::setPosition)
            .property("center", &palka::TransformObject::getCenter, &palka::TransformObject::setCenter)
            .property("scale", &palka::TransformObject::getScale, &palka::TransformObject::setScale)
            .property("rotate", &palka::TransformObject::getRotation, &palka::TransformObject::setRotation);
}
#endif

