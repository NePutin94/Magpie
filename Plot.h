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

        Plot() = default;

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
            palka::Vec2<T> out;
            T det = a * other.b - other.a * b;
            if(det == 0)
                return palka::Vec2<T>{std::numeric_limits<T>::min(), std::numeric_limits<T>::min()};
            out.y = -(other.a * c - a * other.c) / det;
            out.x = -(b * other.c - other.b * c) / det;
            return out;
        }

        std::vector<palka::Vec2<T>> generateAAB(const palka::Vec2<T>& pointOnPlot, T offset = 1, int count = 1) const
        {
            std::vector<palka::Vec2<T>> p1;
            for(int i = 1; i < count + 1; ++i)
            {
                auto max = pointOnPlot + palka::Vec2<T>{offset * i, offset * i};
                auto min = pointOnPlot + palka::Vec2<T>{-offset * i, -offset * i};

                p1.emplace_back(max.x, getValueAtY(max.x));
                p1.emplace_back(getValueAtX(max.y), max.y);
                p1.emplace_back(min.x, getValueAtY(min.x));
                p1.emplace_back(getValueAtX(min.y), min.y);
            }
            return p1;
        }

        bool hasPoint(const palka::Vec2<T>& p) const
        {
            return PlotCompare<T>(a * p.x + p.y * b, c);
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
