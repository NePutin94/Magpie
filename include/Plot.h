#ifndef MAGPIE_PLOT_H
#define MAGPIE_PLOT_H

#include "MatrixStorage.h"
#include <Vec2.h>
#include <optional>

namespace Magpie
{
#if TESTING
#define EPSILON 0.01
#else
#define EPSILON 0.00001
#endif

    template<class T>
    inline bool PlotCompare(T a, T b, T epsilon = EPSILON)
    {
        if constexpr(std::is_integral_v<T>) //to compare int, we lose a unit of precision
            return abs(a - b) <= 2;
        return abs(a - b) <= epsilon; //in some cases, std::numeric_limits<T>::epsilon() is too small a constant
    }

    template<class T>
    class Plot
    {
    public:
        T a;
        T b;
        T c;
        Sign sign;

        Plot() : a(0), b(0), c(0), sign(Sign::EQUAL)
        {}

        Plot(T a, T b, T c) : a(a), b(b), c(c), sign(Sign::EQUAL)
        {}

        Plot(T a, T b, T c, Sign sign) : a(a), b(b), c(c), sign(sign)
        {}

        Plot(const Plot& ot) = default;

        //  auto operator<=>(const Plot<T>&) const = default;

        bool operator==(const Plot& other) const
        {
            return a == other.a && b == other.b && c == other.c && sign == other.sign;
        }

        bool operator!=(const Plot& other) const
        {
            return !(*this == other);
        }

//        bool operator<(const Plot& other) const
//        {
//            return a < other.a && b < other.b && c < other.c && sign < other.sign;
//        }

        bool operator<(const Plot& other) const
        {
            if(a < other.a)
                return true;
            if(a == other.a && b < other.b)
                return true;
            return (a == other.a && b == other.b && c < other.c);
        }

        bool operator<=(const Plot& other) const
        {
            return a <= other.a && b <= other.b && c <= other.c && sign <= other.sign;
        }

        bool operator>(const Plot& other) const
        {
            return a > other.a && b > other.b && c > other.c && sign > other.sign;
        }

        bool operator>=(const Plot& other) const
        {
            return a >= other.a && b >= other.b && c >= other.c && sign >= other.sign;
        }

        bool containsPoint(const palka::Vec2<T>& p, T epsilon = 0.000001) const
        {
            switch((Sign) sign)
            {
                case GREATEROREQUAL:
                    return PlotCompare(p.x * a + p.y * b, c) || p.x * a + p.y * b >= c;
                case LESSOREQUAL:
                    return PlotCompare(p.x * a + p.y * b, c) || p.x * a + p.y * b <= c;
                case EQUAL:
                    return p.x * a + p.y * b == c;
                default:
                    return false;
            }
        }

        auto intersection(const Plot& other) const
        {
            T det = a * other.b - other.a * b;
            if(det == (T)0)
                return palka::Vec2<T>{std::numeric_limits<T>::min(), std::numeric_limits<T>::min()};
            if(b == (T)0)
                return other.intersection(*this);
            auto xt = -a * other.b;
            auto xt2 = -other.a * b;
            auto xtres = xt - xt2;
            auto bres = b * other.c - other.b * c;
            auto xp = bres / xtres;
            auto yp = getValueAtY(xp);
            return palka::Vec2<T>{xp, yp};
        }

        std::vector<palka::Vec2<T>> generateAAB2(const palka::Vec2<T>& pointOnPlot, T offset = 1, int count = 1) const
        {
            std::vector<palka::Vec2<T>> p1;
            if(a == (T)0)
            {
                for(int i = 1; i < count + 1; ++i)
                {
                    auto max = pointOnPlot + palka::Vec2<T>{offset * i, offset * i};
                    auto min = pointOnPlot + palka::Vec2<T>{-offset * i, -offset * i};

                    p1.emplace_back(max.x, getValueAtY(max.x));
                    p1.emplace_back(min.x, getValueAtY(min.x));
                }
            } else if(b == (T)0)
            {
                for(int i = 1; i < count + 1; ++i)
                {
                    auto max = pointOnPlot + palka::Vec2<T>{offset * i, offset * i};
                    auto min = pointOnPlot + palka::Vec2<T>{-offset * i, -offset * i};

                    if(containsPoint({(T)0, (T)0}))
                    {
                        p1.emplace_back(pointOnPlot.x - offset, max.y);
                        p1.emplace_back(pointOnPlot.x - offset, min.y);
                    } else
                    {
                        p1.emplace_back(pointOnPlot.x + offset, max.y);
                        p1.emplace_back(pointOnPlot.x + offset, min.y);
                    }
                }
            } else
            {
                for(int i = 1; i < count + 1; ++i)
                {
                    auto max = pointOnPlot + palka::Vec2<T>{offset * i, offset * i};
                    auto min = pointOnPlot + palka::Vec2<T>{-offset * i, -offset * i};

                    p1.emplace_back(max.x, getValueAtY(max.x));
                    p1.emplace_back(min.x, getValueAtY(min.x));

//                    p1.emplace_back(max.x, getValueAtY(max.x));
//                    p1.emplace_back(getValueAtX(max.y), max.y);
//                    p1.emplace_back(min.x, getValueAtY(min.x));
//                    p1.emplace_back(getValueAtX(min.y), min.y);
                }
            }
            return p1;
        }

        std::vector<palka::Vec2<T>> generateAAB(const palka::Vec2<T>& pointOnPlot, T offset = 1, int count = 1) const
        {
            std::vector<palka::Vec2<T>> p1;

            if(a == (T)0)
            {
                for(int i = 1; i < count + 1; ++i)
                {
                    auto max = pointOnPlot + palka::Vec2<T>{offset * i, offset * i};
                    auto min = pointOnPlot + palka::Vec2<T>{-offset * i, -offset * i};

                    p1.emplace_back(max.x, getValueAtY(max.x));
                    p1.emplace_back(min.x, getValueAtY(min.x));
                }
            } else if(b == (T)0)
            {
                for(int i = 1; i < count + 1; ++i)
                {
                    auto max = pointOnPlot + palka::Vec2<T>{offset * i, offset * i};
                    auto min = pointOnPlot + palka::Vec2<T>{-offset * i, -offset * i};

                    p1.emplace_back(getValueAtX(max.y), max.y);
                    p1.emplace_back(getValueAtX(min.y), min.y);
                }
            } else
            {
                for(int i = 1; i < count + 1; ++i)
                {
                    auto max = pointOnPlot + palka::Vec2<T>{offset * i, offset * i};
                    auto min = pointOnPlot + palka::Vec2<T>{-offset * i, -offset * i};

                    p1.emplace_back(max.x, getValueAtY(max.x));
                    p1.emplace_back(getValueAtX(max.y), max.y);
                    p1.emplace_back(min.x, getValueAtY(min.x));
                    p1.emplace_back(getValueAtX(min.y), min.y);
                }
            }
            return p1;
        }

        Plot getPerpendicular(palka::Vec2<T> p = {0, 0})
        {
            auto yx = -a / b;
            auto newB = 1.f / yx;
            auto newC = p.y + newB * p.x;
            return Plot(newB, 1, newC);
        }

        palka::Vec2<T> getGrad()
        {
            return glm::normalize(palka::Vec2<T>((T) a, (T) b));
        }

        bool hasPoint(const palka::Vec2<T>& p) const
        {
            return PlotCompare<T>(a * p.x + p.y * b, c);
        }

        bool passesOrigin()
        {
            return (a == 0 || b == 0) ? true : getValueAtY(0) ==(T)0;
        }

        T getValue(T x, T y)
        {
            return a * x + y * b;
        }

        T getValueAtY(T x) const
        {
            return (c - a * x) / b;
        }

        T getValueAtX(T y) const
        {
            return (c - b * y) / a;
        }

        T getValueAtXC(T y) const
        {
            return (- b * y) / a;
        }

        T getValueAtYC(T x) const
        {
            return (- a * x) / b;
        }
    };

    template<class T>
    class Plot3D
    {
    public:
        T a;
        T b;
        T c;
        T d;
        Sign sign;

        Plot3D() : a(0), b(0), c(0), d(0), sign(Sign::EQUAL)
        {}

        Plot3D(T a, T b, T c, T d) : a(a), b(b), c(c), d(d), sign(Sign::EQUAL)
        {}

        Plot3D(T a, T b, T c, T d, Sign sign) : a(a), b(b), c(c), d(d), sign(sign)
        {}

        Plot3D(const Plot3D& ot) = default;

        bool operator==(const Plot3D& other) const
        {
            return a == other.a && b == other.b && c == other.c && d == other.d && sign == other.sign;
        }

        bool operator!=(const Plot3D& other) const
        {
            return !(*this == other);
        }

        palka::Vec3<T> getGrad()
        {
            return glm::normalize(palka::Vec3<T>((T) a, (T) b, (T)c));
        }

        palka::Vec3<T> normal() const
        {
            return {a, b, c};
        }

        palka::Vec3<T> DirNormal() const
        {
            if((Sign) sign == LESSOREQUAL)
            {
                return palka::Vec3<T>{a, b, c} * palka::Vec3<T>(-1, -1, -1);
            } else
            {
                return {a, b, c};
            }
        }

        bool containsPoint(const palka::Vec3<T>& p, T epsilon = 0.000001) const
        {
            switch((Sign) sign)
            {
                case GREATEROREQUAL:
                    return PlotCompare(p.x * a + p.y * b + p.z * c, d) || p.x * a + p.y * b + p.z * c >= d;
                case LESSOREQUAL:
                    return PlotCompare(p.x * a + p.y * b + p.z * c, d) || p.x * a + p.y * b + p.z * c <= d;
                case EQUAL:
                    return p.x * a + p.y * b + p.z * c == d;
                default:
                    return false;
            }
        }

        bool on_line(const palka::Vec3<T>& p, T epsilon = 0.000001)
        {
            return PlotCompare(p.x * a + p.y * b + p.z * c, d);
        }

        bool isect_plane_plane_to_normal_ray(const Plot3D& p2,
                                             palka::Vec3<T>& r_point, palka::Vec3<T>& r_normal)
        {
            const palka::Vec3<T> p3_normal = glm::cross(normal(), p2.normal());
            T det = glm::length(p3_normal);
            det *= det;
            if(det != 0.0)
            {
                r_point = (glm::cross(p3_normal, p2.normal()) * (d * (T)-1) +
                           glm::cross(normal(), p3_normal) * (p2.d * (T)-1)) / det;
                r_normal = p3_normal;
                return true;
            } else
            {
                return false;
            }
        }

        static palka::Vec3<T> getIntersection(const Plot3D& plane1, const Plot3D& plane2, const Plot3D& plane3)
        {
            T det = glm::determinant(palka::Mat3f(plane1.normal(), plane2.normal(), plane3.normal()));
            if(det == 0.f) return palka::Vec3<T>{0.f}; //could return inf or whatever
            return (glm::cross(plane2.normal(), plane3.normal()) * -plane1.d +
                    glm::cross(plane3.normal(), plane1.normal()) * -plane2.d +
                    glm::cross(plane1.normal(), plane2.normal()) * -plane3.d) / det;
        }

        static glm::vec3 threePlaneIntersection(Plot3D p0, Plot3D p1, Plot3D p2)
        {
            const auto a0 = p0.a;
            const auto b0 = p0.b;
            const auto c0 = p0.c;
            const auto d0 = p0.z;

            const auto a1 = p1.a;
            const auto b1 = p1.b;
            const auto c1 = p1.c;
            const auto d1 = p1.z;

            const auto a2 = p2.a;
            const auto b2 = p2.b;
            const auto c2 = p2.c;
            const auto d2 = p2.z;

            glm::vec3 intr;
            intr.x = -(-b0 * c1 * d2 + b0 * c2 * d1 + b1 * c0 * d2 - b1 * c2 * d0 - b2 * c0 * d1 + b2 * c1 * d0)
                     / (-a0 * b1 * c2 + a0 * b2 * c1 + a1 * b0 * c2 - a1 * b2 * c0 - a2 * b0 * c1 + a2 * b1 * c0);
            intr.y = -(a0 * c1 * d2 - a0 * c2 * d1 - a1 * c0 * d2 + a1 * c2 * d0 + a2 * c0 * d1 - a2 * c1 * d0)
                     / (-a0 * b1 * c2 + a0 * b2 * c1 + a1 * b0 * c2 - a1 * b2 * c0 - a2 * b0 * c1 + a2 * b1 * c0);
            intr.z = -(-a0 * b1 * d2 + a0 * b2 * d1 + a1 * b0 * d2 - a1 * b2 * d0 - a2 * b0 * d1 + a2 * b1 * d0)
                     / (-a0 * b1 * c2 + a0 * b2 * c1 + a1 * b0 * c2 - a1 * b2 * c0 - a2 * b0 * c1 + a2 * b1 * c0);

            return intr;
        }
    };

    template<class T>
    struct Ray
    {
        palka::Vec3<T> start;
        palka::Vec3<T> dir;

        T checkAxis(T t, T d)
        {
            if(d == 0)
                return std::numeric_limits<T>::min();
            return t / d;
        }

        bool operator==(const Ray& ot) const
        {
            return ot.start == start && glm::normalize(ot.dir) == glm::normalize(dir);
        }

        bool operator!=(const Ray& ot) const
        {
            return !(*this == ot);
        }

        std::optional<palka::Vec3<T>> intersects(Ray r)
        {
            if(glm::normalize(dir) == glm::normalize(r.dir) || glm::normalize(dir) == -glm::normalize(r.dir))
                return {};
            palka::Vec3<T> v1 = dir;
            palka::Vec3<T> v2 = r.dir;
            palka::Vec3<T> v3 = r.start - start;
            auto a = glm::dot(v1, v1);
            auto b = glm::dot(v2, v2);
            auto c = glm::dot(v1, v2);
            auto e = glm::dot(v1, v3);
            auto f = glm::dot(v2, v3);
            auto d = (c * c - a * b);

            if(d * d < (T)0.000001)
                return start;

            T m = (c * f - b * e) / d;

            return start + palka::Vec3<T>{m, m, m} * v1;
        }

        bool containsPoint(palka::Vec3<T> p)
        {
            if(auto t = checkAxis(p.x - start.x, dir.x); t != std::numeric_limits<T>::min())
            {
                if(start + dir * t == p)
                {
                    return true;
                }
            }
            if(auto t = checkAxis((p.z - start.z), dir.z);t != std::numeric_limits<T>::min())
            {
                if(start + dir * t == p)
                {
                    return true;
                }
            }
            if(auto t = checkAxis((p.y - start.y), dir.y);t != std::numeric_limits<T>::min())
            {
                if(start + dir * t == p)
                {
                    return true;
                }
            }
            return false;
        }

        std::optional<palka::Vec3<T>> intersexctRay(Ray& ot)
        {
            if(glm::normalize(dir) == glm::normalize(ot.dir) || glm::normalize(dir) == -glm::normalize(ot.dir))
                return {};
            if(auto t = checkAxis(ot.start.x - start.x, dir.x - ot.dir.x); t != std::numeric_limits<T>::min())
            {
                if(start + dir * t == ot.start + ot.dir * t)
                {
                    return start + dir * t;
                }
            }
            if(auto t = checkAxis((ot.start.z - start.z), dir.z - ot.dir.z);t != std::numeric_limits<T>::min())
            {
                if(start + dir * t == ot.start + ot.dir * t)
                {
                    return start + dir * t;
                }
            }
            if(auto t = checkAxis((ot.start.y - start.y), dir.y - ot.dir.y);t != std::numeric_limits<T>::min())
            {
                if(start + dir * t == ot.start + ot.dir * t)
                {
                    return start + dir * t;
                }
            }
            return {};
        }
    };
}

#endif //MAGPIE_PLOT_H
