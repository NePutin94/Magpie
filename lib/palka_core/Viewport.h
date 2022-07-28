//
// Created by NePutin on 7/5/2021.
//

#ifndef PALKA_VIEWPORT_H
#define PALKA_VIEWPORT_H

#include "config.h"
#include "Vec2.h"
#include "Rect.h"
#include "Transform.h"
#include <utility>

#ifdef REFLECTION_CORE
#include <rttr/type>
#include <rttr/registration_friend>
#endif

namespace palka
{
    class Viewport
    {
#ifdef REFLECTION_CORE
        RTTR_ENABLE()
        RTTR_REGISTRATION_FRIEND
#endif
    private:
        Vec2f size;
        Vec2f center;
        RectF rect;
        float rotation;
        bool needUpdate;
        Transform t;
    public:
        Viewport(const RectF& r) : rect(r), size(r.w, r.h), needUpdate(true), rotation(0), center()
        {

        }

        void reset(const RectF& r)
        {
            center.x = r.left + r.w / 2.f;
            center.y = r.top + r.h / 2.f;
            size.x = r.w;
            size.y = r.h;
            rotation = 0;
        }

        auto getRect()
        {
            return rect;
        }

        void setRotation(float angle)
        {
            rotation = angle;
            needUpdate = true;
        }

        [[nodiscard]] float getRotation() const
        {
            return rotation;
        }

        void setSize(const Vec2f& sz)
        {
            size = sz;
            rect = {rect.left, rect.top, size.x, size.y};
            needUpdate = true;
        }

        [[nodiscard]] const auto& getSize() const
        {
            return size;
        }

        void setCenter(const Vec2f& c)
        {
            center = c;
            needUpdate = true;
        }

        [[nodiscard]] const auto& getCenter()const
        {
            return center;
        }

        void zoom(float f)
        {
            setSize({size.x * f, size.y * f});
        }

        Transform getView()
        {
            if (needUpdate)
            {
                Vec2f m_center = center;
                Vec2f m_size(size.x, size.y);
                float m_rotation = rotation;
                float angle = m_rotation * M_PI / 180.f;
                float cosine = std::cos(angle);
                float sine = std::sin(angle);
                float tx = -m_center.x * cosine - m_center.y * sine + m_center.x;
                float ty = m_center.x * sine - m_center.y * cosine + m_center.y;

                float a = 2.f / m_size.x;
                float b = -2.f / m_size.y;
                float c = -a * m_center.x;
                float d = -b * m_center.y;

                t = Transform(a * cosine, a * sine, a * tx + c,
                              -b * sine, b * cosine, b * ty + d,
                              0.f, 0.f, 1.f);
                needUpdate = false;
            }
            return t;
        }

        Vec2f mapPixelToCoords(const Vec2f& point)
        {
            Vec2f normalized;
            auto width = static_cast<float>(getSize().x);
            auto height = static_cast<float>(getSize().y);
            RectF viewport = {0, 0, width, height};

            normalized.x = -1.f + 2.f * (point.x - viewport.left) / viewport.w;
            normalized.y = 1.f - 2.f * (point.y - viewport.top) / viewport.h;

            return getView().getInverse().transformPoint(normalized);
        }

        Vec2f getStaticOffset()
        {
            const float viewPortScaleX = std::round(((int) 1280 / getSize().x) * 100) / 100;
            const float viewPortScaleY = std::round(((int) 1280 / getSize().y) * 100) / 100;
            Vec2f offset = Vec2f(getSize().x / 2, getSize().y / 2) - getCenter();
            offset.x *= viewPortScaleX;
            offset.y *= viewPortScaleY;
            return offset;
        }
    };
}

#endif //PALKA_VIEWPORT_H
