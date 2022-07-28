//
// Created by NePutin on 8/2/2021.
//

#ifndef PALKA_TRANSFORMOBJECT_H
#define PALKA_TRANSFORMOBJECT_H

#include "Transform.h"

#ifdef REFLECTION_CORE

#include <rttr/type>
#include <rttr/registration_friend>

#endif
namespace palka
{
    class TransformObject
    {
#ifdef REFLECTION_CORE
        RTTR_ENABLE()
        RTTR_REGISTRATION_FRIEND
#endif
    private:
        Vec2f scale{1.f, 1.f};
        float angle{};
        Vec2f pos;
        Vec2f center;
        mutable Transform t;
        mutable bool updateTransform{};
    public:
        TransformObject() = default;

        void setPosition(Vec2f pos);

        Vec2f getPosition() const;

        void setScale(Vec2f scale);

        Vec2f getScale() const;

        void setRotation(float angle);

        float getRotation() const;

        void setCenter(Vec2f center);

        Vec2f getCenter() const;

        void copy(TransformObject t)
        {
            this->scale = t.scale;
            this->angle = t.angle;
            this->pos = t.pos;
            this->center = t.center;
            this->t = t.t;
            this->updateTransform = t.updateTransform;
        }

        Transform getTransform() const;

        Quad<float> getQuad(RectF rect);
    };
}

#endif //PALKA_TRANSFORMOBJECT_H
