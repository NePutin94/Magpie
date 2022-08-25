#ifndef MAGPIE_MATRIXSTORAGE_H
#define MAGPIE_MATRIXSTORAGE_H

#include <vector>
#include <string>
#include <ranges>
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
            data.clear();
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
            } else
            {
                if(ImGui::BeginTable("debugmatrix1", columns))
                {
                    for(int i = 0; i < rows; ++i)
                    {
                        ImGui::TableNextRow();
                        for(int j = 0; j < columns; ++j)
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
}
#endif //MAGPIE_MATRIXSTORAGE_H
