//
// Created by NePutin on 9/18/2021.
//

#ifndef PALKA_RECTANGLE_H
#define PALKA_RECTANGLE_H

#include "Drawable.h"
#include "Rect.h"
#include "TransformObject.h"
#include "VertexData.h"
#include "RenderContext.h"

#ifdef REFLECTION_CORE

#include <rttr/type>
#include <rttr/wrapper_mapper.h>
#include <rttr/registration_friend>
#include "RawPtrWrapper.h"
#include <glm/geometric.hpp>
#endif

namespace palka
{
    class Rectangle : public Drawable, public TransformObject
    {
#ifdef REFLECTION_CORE
        RTTR_ENABLE(TransformObject)
        RTTR_REGISTRATION_FRIEND
#endif

        void update()
        {
            fill[0] = Vertex(Vec2f{rect.left, rect.top});
            fill[1] = Vertex(Vec2f{rect.w, rect.top});
            fill[2] = Vertex(Vec2f{rect.w, rect.h});
            fill[3] = Vertex(Vec2f{rect.left, rect.h});
            int j = 0;
            for (int i = 0; i < 4; ++i)
            {
                Vec2f a = (i == 0) ? fill[3].pos : fill[i - 1].pos;
                Vec2f b = fill[i].pos;
                Vec2f c = (i == 3) ? fill[0].pos : fill[i + 1].pos;

                Vec2f normal = glm::normalize(b - a);
                Vec2f normal2 = glm::normalize(b - c);

                float len = 1.f + glm::length(normal * normal2);
                Vec2f dir = (normal + normal2) / len;
                outline[j++] = Vertex({b});
                outline[j++] = Vertex({b + dir * cornerSize});
            }
            outline[j++] = outline[0];
            outline[j++] = outline[1];
        }

        void updateColor()
        {
            for (auto& v: fill)
                v.color = fillColor;
            for (auto& v: outline)
                v.color = outlineColor;
        }

    public:
        Rectangle(RectF rect,float sz = 2) : rect(rect), outline(VertArray::Triangle_Strip), fill(VertArray::Polygon), cornerSize(sz)
        {
            fill.add(Vec2f{rect.left, rect.top});
            fill.add(Vec2f{rect.w, rect.top});
            fill.add(Vec2f{rect.w, rect.h});
            fill.add(Vec2f{rect.left, rect.h});
            for (int i = 0; i < 4; ++i)
            {
                Vec2f a = (i == 0) ? fill[3].pos : fill[i - 1].pos;
                Vec2f b = fill[i].pos;
                Vec2f c = (i == 3) ? fill[0].pos : fill[i + 1].pos;

                Vec2f normal = glm::normalize(b - a);
                Vec2f normal2 =glm::normalize(b - c);

                float len = 1.f +  glm::length(normal * normal2);
                Vec2f dir = (normal + normal2) / len;
                outline.add({b});
                outline.add({b + dir * sz});
            }
            outline.add(outline[0]);
            outline.add(outline[1]);
        }

        void draw(Renderer& win) const;

        void setOutlineColor(Color outline)
        {
            outlineColor = outline;
            updateColor();
        }

        void setFillColor(Color fill)
        {
            fillColor = fill;
            updateColor();
        }

        Color getFillColor()
        {
            return fillColor;
        }

        Color getOutlineColor()
        {
            return outlineColor;
        }

        RectF getRect()
        {
            return rect;
        }

        void setRect(RectF rect)
        {
            this->rect = rect;
            update();
        }

        void setSize(float size)
        {
            cornerSize = size;
            update();
        }

        float getSize()
        {
            return cornerSize;
        }

    private:
        float cornerSize;
        RectF rect;
        Color outlineColor;
        Color fillColor;
        VertArray outline;
        VertArray fill;
    };
}

#endif //PALKA_RECTANGLE_H
