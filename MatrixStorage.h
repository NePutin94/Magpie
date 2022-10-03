#ifndef MAGPIE_MATRIXSTORAGE_H
#define MAGPIE_MATRIXSTORAGE_H

#include <vector>
#include <string>
#include <ranges>
#include <fmt/format.h>
#include "Fractus.h"
#include "Vec2.h"
#include <imgui.h>

namespace Magpie
{
    enum Sign
    {
        EQUAL = 0,
        GREATEROREQUAL = -1,
        LESSOREQUAL = 1
    };

    inline constexpr std::string_view signToStr(Sign s)
    {
        constexpr std::string_view arr[] = {"=", ">=", "<="};
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

    //Agreement:
    //first row of the matrix is the objective function: x_1 x_2 ... x_n
    //remaining lines are constraints: x_1 x_2 [<=, =, >=] x_3; For the nXm matrix sign of the constraint can be obtained: matrix[n][m-1]
    //number of columns is determined by the longest row
    template<class T>
    class MatrixStorage // row major matrix, matrix for storing task conditions
    {
    private:
        std::vector<T> data;
        size_t rows;
        size_t columns;
        static constinit const short cap = 4;
    public:
        MatrixStorage() : rows(0), columns(0)
        {}

        MatrixStorage(size_t row, size_t column)
        { alloc_matrix(row, column); }

        MatrixStorage(const std::vector<std::vector<T>>& vec)
        {
            rows = vec.size();
            columns = vec[0].size();
            for(auto& v: vec)
            {
                for(auto v2: v)
                    data.emplace_back(v2);
            }
        }

        MatrixStorage(std::vector<T>&& vec, size_t rows, size_t columns)
        {
            this->rows = rows;
            this->columns = columns;
            data = std::move(vec);
        }

        MatrixStorage(const std::vector<T>& vec, size_t rows, size_t columns)
        {
            this->rows = rows;
            this->columns = columns;
            data = vec;
        }

        MatrixStorage(const std::vector<std::vector<T>>& vec, size_t rows, size_t columns)
        {
            rows = vec.size();
            columns = vec[0].size();
            alloc_matrix(rows, columns);
            for(auto& v: vec)
            {
                for(auto v2: v)
                    data.emplace_back(v2);
            }
        }

        auto getRow(size_t pos)
        {
            std::vector<T> vec;
            std::ranges::copy(data | std::ranges::views::drop(columns * pos) | std::ranges::views::take(columns),
                              std::back_inserter(vec));
            return vec;
        }

        auto dropRow(size_t pos) const
        {
            auto del = [](int delPos, int c)
            {
                return [i = -1, delPos, c](auto const&) mutable
                {
                    i++;
                    if(i / c == delPos - 1)
                        return false;
                    return true;
                };
            };
            auto rng = data | std::ranges::views::filter(del(pos, columns));
            std::vector<T> v;
            std::ranges::copy(rng, std::back_inserter(v));
            return MatrixStorage{v, this->rows - 1, this->columns};
        }

        auto dropColumn(size_t pos) const
        {
            auto del = [](size_t delPos, size_t c)
            {
                return [i = -1, delPos, c](auto const&) mutable
                {
                    i++;
                    if(i % c == delPos - 1)
                        return false;
                    return true;
                };
            };
            auto rng = data | std::ranges::views::filter(del(pos, columns));
            std::vector<T> v;
            std::ranges::copy(rng, std::back_inserter(v));
            return MatrixStorage{v, this->rows, this->columns - 1};
        }

        void alloc_matrix(size_t row, size_t column)
        {
            rows = row;
            columns = column;
            data.clear();
            data.resize(rows * columns);
        }

        std::vector<std::vector<T>> getMatrix() const
        {
            std::vector<std::vector<T>> res;
            res.resize(rows);
            for(auto& v: res)
                v.resize(columns - 1);

            for(int i = 1; i < rows; ++i)
            {
                for(int j = 0, j2 = 0; j < columns; ++j)
                {
                    if(j == columns - 2) continue;
                    res[i - 1][j2++] = get(j, i);
                }
            }
            for(int j = 0, j2 = 0; j < columns; ++j)
            {
                if(j == columns - 2) continue;
                res[rows - 1][j2++] = get(j, 0);
            }
            return res;
        }

        void setMatrix(const std::vector<std::vector<T>>& matrix)
        {
            alloc_matrix(matrix.size(), matrix[0].size());
            for(int j = 0, j2 = 0; j < columns; ++j)
            {
                get(j, 0) = matrix[rows - 1][j2++];
            }
            for(int i = 1; i < rows; ++i)
            {
                for(int j = 0, j2 = 0; j < columns; ++j)
                {
                    get(j, i) = matrix[i - 1][j2++];
                }
            }
        }

        void add(T val, size_t column, size_t row)
        {
            if(row * columns + column >= data.size())
                data.resize((row * columns + column) - data.size() + cap);
            get(column, row) = val;
        }

        auto rows_count() const
        { return rows; }

        auto columns_count() const
        { return columns; }

        const T& get(size_t column, size_t row) const
        {
            return data[row * columns + column];
        }

        T& get(size_t column, size_t row)
        {
            return data[row * columns + column];
        }

        void debug() const
        {
            ImGui::Begin(fmt::format("##debugmatrix{}", fmt::ptr(this)).c_str());
            if constexpr(std::is_integral_v<T>)
            {
                if(ImGui::BeginTable(fmt::format("##debugmatrix{}", fmt::ptr(this)).c_str(), columns))
                {
                    for(size_t i = 0; i < rows; ++i)
                    {
                        ImGui::TableNextRow();
                        for(size_t j = 0; j < data.size() / rows; ++j)
                        {
                            ImGui::TableNextColumn();
                            ImGui::Text("%i", get(j, i));
                        }
                    }
                    ImGui::EndTable();
                }
            } else
            {
                if(ImGui::BeginTable(fmt::format("##debugmatrix{}", fmt::ptr(this)).c_str(), columns))
                {
                    for(size_t i = 0; i < rows; ++i)
                    {
                        ImGui::TableNextRow();
                        for(size_t j = 0; j < columns; ++j)
                        {
                            ImGui::TableNextColumn();
                            ImGui::Text("%f", get(j, i));
                        }
                    }
                    ImGui::EndTable();
                }
            }
            if constexpr(std::is_same_v<T, Fractus>)
            {
                if(ImGui::BeginTable(fmt::format("##debugmatrix{}", fmt::ptr(this)).c_str(), columns))
                {
                    for(size_t i = 0; i < rows; ++i)
                    {
                        ImGui::TableNextRow();
                        for(size_t j = 0; j < data.size() / rows; ++j)
                        {
                            ImGui::TableNextColumn();
                            // ImGui::Text("%i %i", get(j, i));
                        }
                    }
                    ImGui::EndTable();
                }
            }
            ImGui::End();
        }
    };
}
#endif //MAGPIE_MATRIXSTORAGE_H
