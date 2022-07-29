//
// Created by dimka on 2/3/2022.
//

#ifndef BILLET_SOLVER_H
#define BILLET_SOLVER_H

#include <utility>
#include <vector>
#include <cassert>
#include <valarray>
#include <ranges>
#include "imgui.h"
#include "Vec2.h"

namespace Magpie
{
    class Fractus
    {
    private:
        long int denominator;
        long int numerator;

        [[nodiscard]] int gcd(int a, int b) const
        {
            if(a == 0)
                return b;
            return gcd(b % a, a);
        }

    public:
        Fractus(long int numerator, long int denominator) : numerator(numerator), denominator(denominator)
        {

        }

        Fractus operator+(const Fractus& other) const
        {
            auto n1 = numerator * other.denominator;
            auto n2 = other.numerator * denominator;
            long int new_den = denominator * other.denominator, new_num = n1 + n2;
            auto d = gcd(new_den, new_num);
            new_den /= d;
            new_num /= d;
            return Fractus{new_num, new_den};
        }

        Fractus operator-(const Fractus& other) const
        {
            long int new_den, new_num;
            auto n1 = numerator * other.denominator;
            auto n2 = other.numerator * denominator;
            return Fractus{n1 - n2, denominator * other.denominator};
        }

        template<class U>
        void convertTO()
        requires std::is_floating_point_v<U>
        {

            return (U) numerator / denominator;
        }

        template<class U>
        void convertFROM(U number, int cycles = 10, double precision = 5e-4)
        requires std::is_floating_point_v<U>
        {
            int sign = number > 0 ? 1 : -1;
            number = number * sign; //abs(number);
            double new_number, whole_part;
            double decimal_part = number - (int) number;
            int counter = 0;

            std::valarray<double> vec_1{double((int) number), 1}, vec_2{1, 0}, temporary;

            while(decimal_part > precision & counter < cycles)
            {
                new_number = 1 / decimal_part;
                whole_part = (int) new_number;

                temporary = vec_1;
                vec_1 = whole_part * vec_1 + vec_2;
                vec_2 = temporary;

                decimal_part = new_number - whole_part;
                counter += 1;
            }
            numerator = sign * vec_1[0];
            denominator = vec_1[1];
        }
    };


    template<class T>
    class MatrixStorage // row major matrix
    {
    private:
        std::vector<T> data;
        size_t rows;
        size_t columns;
        static constinit const short cap = 4;
    public:
        MatrixStorage(size_t row, size_t column)
        { alloc_matrix(row, column); }

        MatrixStorage(const std::vector<T>& data, size_t row, size_t column) : rows(row), columns(column)
        {
            data = data;
//            alloc_matrix(row, column);
//            int index = 0;
//            for(int i = 0; i < rows; ++i)
//                for(int j = 0; j < column; ++j)
//                    get(j, i) = data[index++];
        }

        auto get_col(size_t col)
        {
            auto stride = [](int n)
            {
                return [s = -1, n](auto const&) mutable
                {
                    s = (s + 1) % n;
                    return !s;
                };
            };
            return data | std::ranges::views::drop(col) | std::ranges::views::filter(stride(columns));
        }

        void alloc_matrix(size_t row, size_t column)
        {
            rows = row;
            columns = column;
            data.resize(rows * columns);
        }

        void add_row(std::vector<T> row)
        {
//            auto result_data = std::valarray<T>(columns * (rows + 1));
//            result_data[std::slice(0, rows * columns, 1)] = data;
//            auto test = std::valarray<T>(row.data(), columns);
//            result_data[std::slice(rows * columns, columns, 1)] = test;
//            data = result_data;
        }

        void add(T val, size_t column, size_t row)
        {
            if(row * columns + column >= data.size())
                data.resize((row * columns + column) - data.size() + cap);
            get(column, row) = val;
        }

        const T& get(size_t column, size_t row) const
        {
            return data[row * columns + column];
        }

        T& get(size_t column, size_t row)
        {
            return data[row * columns + column];
        }

        void debug()
        {
            if constexpr(std::is_integral_v<T>)
            {
                if(ImGui::BeginTable("debugmatrix1", columns))
                {
                    for(int i = 0; i < rows; ++i)
                    {
                        ImGui::TableNextRow();
                        for(int j = 0; j < data.size() / rows; ++j)
                        {
                            ImGui::TableNextColumn();
                            ImGui::Text("%i", get(j, i));
                        }
                    }
                    ImGui::EndTable();
                }
            }
            if constexpr(std::is_same_v<T, Fractus>)
            {
                if(ImGui::BeginTable("debugmatrix2", columns))
                {
                    for(int i = 0; i < rows; ++i)
                    {
                        ImGui::TableNextRow();
                        for(int j = 0; j < data.size() / rows; ++j)
                        {
                            ImGui::TableNextColumn();
                            // ImGui::Text("%i %i", get(j, i));
                        }
                    }
                    ImGui::EndTable();
                }
            }
        }
    };

    enum Sign
    {
        EQUAL = 0,
        GREATEROREQUAL = -1,
        LESSOREQUAL = 1
    };

    inline std::string signToStr(Sign s)
    {
        std::string arr[] = {"=", ">=", "<="};
        switch(s)
        {
            case EQUAL:
                return arr[0];
                break;
            case GREATEROREQUAL:
                return arr[1];
                break;
            case LESSOREQUAL:
                return arr[2];
                break;
        }
        return "";
    }

    template<class T>
    inline bool PlotCompare(T a, T b)
    {
        if constexpr(std::is_integral_v<T>) //to compare int, we lose a unit of precision
        {
            return std::abs(a - b) <= 1;
        }
        return std::abs(a - b) <= std::numeric_limits<T>::epsilon();
    }

    template<class T>
    class Plot2
    {
    public:
        T a;
        T b;
        T c;
        int sign;

        Plot2() = default;

        Plot2(T a, T b, T c) : a(a), b(b), c(c)
        {}

        Plot2(T a, T b, T c, int S) : a(a), b(b), c(c), sign(S)
        {}

        Plot2(const Plot2& ot) = default;
//        palka::Vec2f getValueAt(float x, float y)
//        {
//            palka::Vec2f out;
//            out.x = (c - b * y) / a;
//            out.y = (c - a * x) / b;
//            return out;
//        }

        auto operator<=>(const Plot2<T>&) const = default;

        bool containsPoint(palka::Vec2<T> p)
        {
            switch((Sign) sign)
            {
                case GREATEROREQUAL:
                    return p.x * a + p.y * b >= c;
                case LESSOREQUAL:
                    return p.x * a + p.y * b <= c;
                case EQUAL:
                    return p.x * a + p.y * b == c;
                default:
                    return false;
            }
        }

        bool hasPoint(palka::Vec2<T> p)
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

    template<class T, class Result>
    class Solver
    {
    private:
    protected:
    public:
        virtual Result solve() = 0;
    };

    class GraphicsResult
    {
    private:
        std::vector<palka::Vec2f> Union;
        palka::Vec2f resultPoint;
        double resultValue;
    public:
        GraphicsResult(std::vector<palka::Vec2f> Union, const palka::Vec2f& resultPoint, double resultValue) : Union(std::move(Union)),
                                                                                                               resultPoint(resultPoint),
                                                                                                               resultValue(resultValue)
        {

        }

        std::pair<std::vector<double>, std::vector<double>> getVisualUnion()
        {
            std::vector<double> X;
            std::vector<double> Y;
            for(auto vec: Union)
            {
                X.emplace_back(vec.x);
                Y.emplace_back(vec.y);
            }
            X.emplace_back(Union.front().x);
            X.emplace_back(Union.front().y);
            return {X, Y};
        }

        palka::Vec2f getResPoint()
        {
            return resultPoint;
        }

        double getResultValue()
        {
            return resultValue;
        }
    };


    template<class T>
    class GraphicMet2D : Solver<T, GraphicsResult>
    {
    private:
        Magpie::Plot2<T> F;
        std::vector<Magpie::Plot2<T>> testArr;
        std::vector<palka::Vec2f> Union;
        using f_pair = std::pair<T, T>;

        auto intersection(f_pair A, f_pair B, f_pair C)
        {
            f_pair out;
            float det = A.first * B.second - A.second * B.first;
            out.second = -(A.second * C.first - A.first * C.second) / det;
            out.first = -(B.first * C.second - B.second * C.first) / det;
            return out;
        }

    public:
        GraphicMet2D() = default;

        GraphicMet2D(const MatrixStorage<T>& data, int var_count, int limitations_count)
        {
            F = Magpie::Plot2<T>{data.get(0, 0), data.get(1, 0), 0};
            for(int i = 1; i < limitations_count; ++i)
            {
                auto a = data.get(0, i);
                auto b = data.get(1, i);
                auto c = data.get(3, i);
                auto s = (int) data.get(2, i);
                testArr.emplace_back(Magpie::Plot2<T>{a, b, c, s});
            }
            testArr.emplace_back(Magpie::Plot2<T>{1, 0, 0, Sign::GREATEROREQUAL});
            testArr.emplace_back(Magpie::Plot2<T>{0, 1, 0, Sign::GREATEROREQUAL});
        }

        void init(const MatrixStorage<T>& data, int var_count, int limitations_count)
        {
            testArr.clear();
            F = Magpie::Plot2<T>{data.get(0, 0), data.get(1, 0), 0};
            for(int i = 1; i < limitations_count + 1; ++i)
            {
                auto a = data.get(0, i);
                auto b = data.get(1, i);
                auto c = data.get(3, i);
                auto s = (int) data.get(2, i);
                testArr.emplace_back(Magpie::Plot2<T>{a, b, c, s});
            }
            testArr.emplace_back(Magpie::Plot2<T>{1, 0, 0, Sign::GREATEROREQUAL});
            testArr.emplace_back(Magpie::Plot2<T>{0, 1, 0, Sign::GREATEROREQUAL});
        }

        GraphicsResult solve() override
        {
            std::vector<palka::Vec2f> points;
            for(int i = 0; i < testArr.size(); ++i) //find all the intersection points
            {
                for(int j = i + 1; j < testArr.size(); ++j)
                {
                    auto& v = testArr[i];
                    auto& v2 = testArr[j];
                    if(v != v2)
                    {
                        auto in = intersection({v.a, v2.a}, {v.b, v2.b}, {v.c, v2.c});
                        if(in.first >= 0 && in.second >= 0)
                        {
                            points.emplace_back(palka::Vec2f{in.first, in.second});
                        }
                    }
                }
            }

            for(auto& p: points) //discard those that do not lie in the domain of each function
            {
                bool all = true;
                for(auto& val: testArr)
                {
                    switch((Sign) val.sign)
                    {
                        case LESSOREQUAL:
                            if(val.a * p.x + val.b * p.y > val.c)
                            {
                                all = false;
                                break;
                            }
                            break;
                        case GREATEROREQUAL:
                            if(val.a * p.x + val.b * p.y < val.c)
                            {
                                all = false;
                                break;
                            }
                            break;
                        case EQUAL:
                            if(val.a * p.x + val.b * p.y != val.c)
                            {
                                all = false;
                                break;
                            }
                            break;
                    }

                }
                if(all)
                {
                    if(std::find_if(Union.begin(), Union.end(), [&](const palka::Vec2f r)
                    { return r == palka::Vec2f{abs(p.x), abs(p.y)}; })
                       == Union.end())
                    {
                        Union.emplace_back(abs(p.x), abs(p.y));
                    }
                }
            }
            std::sort(Union.begin(), Union.end(), [](palka::Vec2f& l, palka::Vec2f& r) //sorting for correct visualization in the future
            {
                return glm::length(l) < glm::length(r);
            });

            std::vector<std::pair<palka::Vec2f, palka::Vec2f>> holes; //TODO
            for(int i = 1; i < Union.size(); ++i)
            {
                auto p1 = Union[i - 1];
                auto p2 = Union[i];
                bool once = false;
                for(auto& l: testArr)
                {
                    if(l.hasPoint(p1) && l.hasPoint(p2))
                    {
                        once = true;
                    }
                }
                if(!once)
                {
                    holes.emplace_back(p1, p2);
                }
            }

            float min = FLT_MAX;
            palka::Vec2f resVec;
            for(auto vec: Union) //find min point and value
            {
                if(auto val = F.a * vec.x + F.a * vec.y; val < min)
                {
                    min = F.a * vec.x + F.a * vec.y;
                    resVec = vec;
                }
            }
            double resValue = min;

            return {Union, resVec, resValue};
        }
    };
}

#endif //BILLET_SOLVER_H
