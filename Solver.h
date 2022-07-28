//
// Created by dimka on 2/3/2022.
//

#ifndef BILLET_SOLVER_H
#define BILLET_SOLVER_H

#include <vector>
#include <cassert>
#include <valarray>
#include <ranges>
#include "imgui.h"
#include "Vec2.h"

namespace Billet
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
            if constexpr (std::is_integral_v<T>)
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
            if constexpr (std::is_same_v<T, Fractus>)
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

    template<class T>
    class Plot2
    {
    public:
        T a;
        T b;
        T c;
        int sign;

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

        bool containsValue()
        {

        }

        T getValue(int x, int y)
        {
            return a * x + y * b;
        }

        float getValueAtY(float x) const
        {
            return (c - a * x) / b;
        }

        float getValueAtX(float y) const
        {
            return (c - b * y) / a;
        }
    };

    template<class T>
    class Solver
    {
    private:
    protected:
    public:
    };

    template<class T>
    class GraphicMet2D
    {
    private:
        MatrixStorage<T> data;
        int var_count;
        int limitations_count;
    public:
        GraphicMet2D(const MatrixStorage<T>& data, int var_count, int limitations_count) : var_count(var_count), limitations_count(limitations_count)
        {
            data = data;
        }

        GraphicMet2D(std::vector<T> target_func, std::vector<T> limiters, std::vector<short> res_sign)
        {
        }

        void solve()
        {

        }
    };
}

#endif //BILLET_SOLVER_H
