#ifndef MAGPIE_DATASTORAGE_H
#define MAGPIE_DATASTORAGE_H

#include <string>
#include "MatrixStorage.h"
#include <functional>
#include <array>

namespace Magpie
{
    class UniversalInput
    {
        std::string val;
    public:
        UniversalInput() = default;

        template<class T>
        UniversalInput(const T& value)
        {
            val = std::to_string(value);
        }

        std::string getValueNotNull()
        {
            if(val.empty())
                return "0";
            return val;
        }

        std::string& getValue()
        {
            return val;
        }
    };

    struct DataStorage
    {
        MatrixStorage<UniversalInput> data;
        MatrixStorage<double> data_work;
        enum DataType
        {
            DOUBLE,
            FLOAT,
            FRACTUS
        };
        std::array<std::string_view, 3> types{"double", "float", "fractus"};
        DataType type = DOUBLE;

        template<class T>
        auto getData()
        {
            MatrixStorage<T> fractus;
            fractus.alloc_matrix(data.rows_count(), data.columns_count());
            for(int i = 0; i < fractus.rows_count(); ++i)
            {
                for(int j = 0; j < fractus.columns_count(); ++j)
                {
                    if constexpr(std::is_same_v<T, float>)
                    {
                        fractus.get(j, i) = std::stof(data.get(j, i).getValueNotNull());
                    } else if constexpr(std::is_same_v<T, double>)
                    {
                        fractus.get(j, i) = std::stod(data.get(j, i).getValueNotNull());
                    } else if constexpr(std::is_same_v<T, Fractus>)
                    {
                        fractus.get(j, i) = Fractus(data.get(j, i).getValueNotNull());
                    }
                }
            }
            return fractus;
        }

        MatrixStorage<double> getDouble()
        {
            MatrixStorage<double> fractus;
            fractus.alloc_matrix(data.rows_count(), data.columns_count());
            for(int i = 0; i < fractus.rows_count(); ++i)
            {
                for(int j = 0; j < fractus.columns_count(); ++j)
                {
                    fractus.get(j, i) = std::stod(data.get(j, i).getValueNotNull());
                }
            }
            return fractus;
        }

        auto getFractus()
        {
            MatrixStorage<Fractus> fractus;
            fractus.alloc_matrix(data.rows_count(), data.columns_count());
            for(int i = 0; i < fractus.rows_count(); ++i)
            {
                for(int j = 0; j < fractus.columns_count(); ++j)
                {
                    fractus.get(j, i) = Fractus(data.get(j, i).getValueNotNull());
                }
            }
            return fractus;
        }
    };
}

#endif //MAGPIE_DATASTORAGE_H
