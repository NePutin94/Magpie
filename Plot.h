#ifndef MAGPIE_PLOT_H
#define MAGPIE_PLOT_H

#include "MatrixStorage.h"

namespace Magpie
{
#if TESTING
#define EPSILON 0.01
#else
#define EPSILON 0.0000001
#endif

    template<class T>
    inline bool PlotCompare(T a, T b, T epsilon = EPSILON)
    {
        if constexpr(std::is_integral_v<T>) //to compare int, we lose a unit of precision
            return std::abs(a - b) <= 2;
        return std::abs(a - b) <= epsilon; //in some cases, std::numeric_limits<T>::epsilon() is too small a constant
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

        auto operator<=>(const Plot<T>&) const = default;

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
            if(det == 0)
                return palka::Vec2<T>{std::numeric_limits<T>::min(), std::numeric_limits<T>::min()};
            auto xt = -a * other.b;
            auto xt2 = -other.a * b;
            auto xtres = xt - xt2;
            auto bres = b * other.c - other.b * c;
            auto xp = bres / xtres;
            auto yp = getValueAtY(xp);
            return palka::Vec2<T>{xp, yp};
            palka::Vec2<T> out;
            out.y = -(other.a * c - a * other.c) / det;
            out.x = -(b * other.c - other.b * c) / det;
            return out;
        }

        std::vector<palka::Vec2<T>> generateAAB2(const palka::Vec2<T>& pointOnPlot, T offset = 1, int count = 1) const
        {
            std::vector<palka::Vec2<T>> p1;
            if(a == 0)
            {
                for(int i = 1; i < count + 1; ++i)
                {
                    auto max = pointOnPlot + palka::Vec2<T>{offset * i, offset * i};
                    auto min = pointOnPlot + palka::Vec2<T>{-offset * i, -offset * i};

                    p1.emplace_back(max.x, getValueAtY(max.x));
                    p1.emplace_back(min.x, getValueAtY(min.x));
                }
            } else if(b == 0)
            {
                for(int i = 1; i < count + 1; ++i)
                {
                    auto max = pointOnPlot + palka::Vec2<T>{offset * i, offset * i};
                    auto min = pointOnPlot + palka::Vec2<T>{-offset * i, -offset * i};

                    if(containsPoint({0, 0}))
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
                    p1.emplace_back(getValueAtX(max.y), max.y);
                    p1.emplace_back(min.x, getValueAtY(min.x));
                    p1.emplace_back(getValueAtX(min.y), min.y);
                }
            }
            return p1;
        }

        std::vector<palka::Vec2<T>> generateAAB(const palka::Vec2<T>& pointOnPlot, T offset = 1, int count = 1) const
        {
            std::vector<palka::Vec2<T>> p1;

            if(a == 0)
            {
                for(int i = 1; i < count + 1; ++i)
                {
                    auto max = pointOnPlot + palka::Vec2<T>{offset * i, offset * i};
                    auto min = pointOnPlot + palka::Vec2<T>{-offset * i, -offset * i};

                    p1.emplace_back(max.x, getValueAtY(max.x));
                    p1.emplace_back(min.x, getValueAtY(min.x));
                }
            } else if(b == 0)
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
            return glm::normalize(palka::Vec2<T>{a, b});
        }

        bool hasPoint(const palka::Vec2<T>& p) const
        {
            return PlotCompare<T>(a * p.x + p.y * b, c);
        }

        bool passesOrigin()
        {
            return (a == 0 || b == 0) ? true : getValueAtY(0) == 0;
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
    };

}

#endif //MAGPIE_PLOT_H
