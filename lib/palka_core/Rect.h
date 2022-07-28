//
// Created by NePutin on 6/23/2021.
//

#ifndef PALKA_RECT_H
#define PALKA_RECT_H


#include "Vec2.h"
#include <utility>

#ifdef REFLECTION_CORE

#include <rttr/type>
#include <rttr/registration_friend>

#endif

namespace palka
{
    template<class T>
    class Rect;

    template<class T>
    struct Quad
    {
        Quad() = default;

        glm::vec<2,T,glm::defaultp> leftTop, rightTop, leftBottom, rightBottom;
        explicit Quad(std::array<glm::vec<2,T,glm::defaultp>, 4> arr);

        explicit Quad(Rect<T> r);

        Quad<int> toInt()
        {
            Quad<int> q;
//            q.leftTop = Vec2i(leftTop.x, leftTop.y);
//            q.rightTop = Vec2i(rightTop.x, rightTop.y);
//            q.leftBottom = Vec2i(leftBottom.x, leftBottom.y);
//            q.rightBottom = Vec2i(rightBottom.x, rightBottom.y);
            return q;
        }
    };

    template<class T>
    class Rect
    {
    public:
        T left, top, w, h;

        Rect() = default;

        Rect(T x, T y, T w, T h) noexcept : left(x), top(y), w(w), h(h)
        {}

        Rect(const Rect&) = default;

        static constexpr Rect Zero()
        {
            return Rect((T) 0, (T) 0, (T) 0, (T) 0);
        }

        template<class T2>
        bool contains(glm::vec<2,T2,glm::defaultp>);

        bool intersects(Rect, Rect&);

        std::array<glm::vec<2,T,glm::defaultp>, 4> getPoints() const
        {

            return {glm::vec<2,T,glm::defaultp>{this->left, this->top},
                    glm::vec<2,T,glm::defaultp>{this->left + this->w, this->top},
                    glm::vec<2,T,glm::defaultp>{this->left, this->top + this->h},
                    glm::vec<2,T,glm::defaultp>{this->left + this->w, this->top + this->h}};
        }

        Quad<T> rotate(float angle, Vec2i origin)
        {
//            palka::Matrix2DRotate<float> mat2(angle);
//            auto points = getPoints();
//            Vec2<T> top1 = points[0];
//            Vec2<T> top2 = points[1];
//            Vec2<T> bottom1 = points[2];
//            Vec2<T> bottom2 = points[3];
//            top1 = top1 - origin;
//            top2 = top2 - origin;
//            bottom1 = bottom1 - origin;
//            bottom2 = bottom2 - origin;
//            top1 = mat2 * top1;
//            top2 = mat2 * top2;
//            bottom1 = mat2 * bottom1;
//            bottom2 = mat2 * bottom2;
//            top1 += origin;
//            top2 += origin;
//            bottom1 += origin;
//            bottom2 += origin;
           // return Quad<T>({top1, top2, bottom1, bottom2});
           return {};
        }

        bool operator==(Rect other);

        bool operator!=(Rect other);
    };

    template<class T>
    Quad<T>::Quad(Rect<T> r)
    {
        auto arr = r.getPoints();
        leftTop = arr[0];
        rightTop = arr[1];
        leftBottom = arr[2];
        rightBottom = arr[3];
    }

    template<class T>
    Quad<T>::Quad(std::array<glm::vec<2,T,glm::defaultp>, 4> arr)
    {
        leftTop = arr[0];
        rightTop = arr[1];
        leftBottom = arr[2];
        rightBottom = arr[3];
    }

    template<class T>
    template<class T2>
    bool Rect<T>::contains(glm::vec<2,T2,glm::defaultp> vec)
    {
        T minX = std::min(left, static_cast<T>(left + w));
        T maxX = std::max(left, static_cast<T>(left + w));
        T minY = std::min(top, static_cast<T>(top + h));
        T maxY = std::max(top, static_cast<T>(top + h));
        return (vec.x >= minX) && (vec.x < maxX) && (vec.y >= minY) && (vec.y < maxY);
    }

    template<class T>
    bool Rect<T>::intersects(Rect<T> r2, Rect<T>& intersection)
    {
        T r1MinX = std::min(left, static_cast<T>(left + w));
        T r1MaxX = std::max(left, static_cast<T>(left + w));
        T r1MinY = std::min(top, static_cast<T>(top + h));
        T r1MaxY = std::max(top, static_cast<T>(top + h));

        T r2MinX = std::min(r2.left, static_cast<T>(r2.left + r2.w));
        T r2MaxX = std::max(r2.left, static_cast<T>(r2.left + r2.w));
        T r2MinY = std::min(r2.top, static_cast<T>(r2.top + r2.h));
        T r2MaxY = std::max(r2.top, static_cast<T>(r2.top + r2.h));

        T interLeft = std::max(r1MinX, r2MinX);
        T interTop = std::max(r1MinY, r2MinY);
        T interRight = std::min(r1MaxX, r2MaxX);
        T interBottom = std::min(r1MaxY, r2MaxY);

        if ((interLeft < interRight) && (interTop < interBottom))
        {
            intersection = Rect<T>(interLeft, interTop, interRight - interLeft, interBottom - interTop);
            return true;
        } else
        {
            intersection = Rect<T>(0, 0, 0, 0);
            return false;
        }
    }

    template<class T>
    bool Rect<T>::operator==(Rect other)
    {
        return this->left == other.left && this->top == other.top && this->w == other.w && this->h == other.h;
    }

    template<class T>
    bool Rect<T>::operator!=(Rect other)
    {
        return !((*this) == other);
    }

    using RectF = Rect<float>;
    using RectI = Rect<int>;
}

#endif //PALKA_RECT_H
