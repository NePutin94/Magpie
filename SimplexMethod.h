#ifndef MAGPIE_SIMPLEXMETHOD_H
#define MAGPIE_SIMPLEXMETHOD_H

#include "ISolver.h"
#include "MatrixStorage.h"
#include <Tracy.hpp>

namespace Magpie
{
    template<class T>
    class SimplexResult
    {
    private:
    public:
    };
    template<class T>
    inline bool compare_float(T x, T y, T epsilon = 0.001f)
    {
        if (fabs(x - y) < epsilon)
            return true;
        return false;
    }
    template<class T>
    class SimplexMethod : public Solver<T, SimplexResult<T>>
    {
    public:
        MatrixStorage<T> input;
        MatrixStorage<T> data; //all solve function
        std::vector<size_t> bazisVars;
        std::vector<T> target_function;
        int _varCount;
        int _limitedCount;
        std::map<int, int> orderCoefficients;

        void makeCanonicalForm()
        {
            MatrixStorage<T> result;
            std::map<int, int> idsNewVar;
            for(int i = 0; i < data.rows_count(); ++i) //search for rows with inequality signs
                if(auto sig = (Sign) data.get(data.columns_count() - 2, i); sig != Sign::EQUAL)
                    idsNewVar.emplace(i, (sig == Sign::LESSOREQUAL) ? 1 : -1);

            data = data.dropColumn(data.columns_count() - 1); //inequality signs are no longer needed

            if(!idsNewVar.empty())
            {
                result.alloc_matrix(data.rows_count() + 1, data.columns_count() + idsNewVar.size()); //a matrix with space for new variables and deltas

                int placeOffset = data.columns_count() - 1; //without b coefficient
                for(int i = 0; i < data.rows_count(); ++i)
                {
                    bool rowHasNewVar = false;
                    std::pair<int, int> newVar = {-1, 1};
                    if(rowHasNewVar = idsNewVar.contains(i);rowHasNewVar)
                        newVar = *idsNewVar.find(i);
                    for(int j = 0; j < data.columns_count() + idsNewVar.size() - 1; ++j)
                    {
                        if(j == data.columns_count() - 1) //b at the end, after new vars
                        {
                            result.get(data.columns_count() + idsNewVar.size() - 1, i) = data.get(j, i) * newVar.second;
                        }
                        if(j < data.columns_count() - 1) //without b coefficient
                            result.get(j, i) = data.get(j, i) * newVar.second;
                        else if(rowHasNewVar && j == placeOffset) //add new var
                            result.get(j, newVar.first) = 1;
                        else
                            result.get(j, newVar.first) = 0;
                    }
                    if(rowHasNewVar)
                        placeOffset++;
                }
            }
            data = result;
            basisFormation({3, 4, 5});
            for(int j = 0; j < idsNewVar.size(); ++j)
                target_function.emplace_back(0);
            removeNegativeB();
            deltasCalculation();
        }

        void deltasCalculation()
        {
            for(int i = 0; i < data.columns_count(); ++i)
            {
                T sum = 0;
                for(int j = 0; j < data.rows_count() - 1; ++j)
                {
                    T a = target_function[orderCoefficients[j]];
                    T b = data.get(i, j);
                    auto res = a * b;
                    sum += res;
                }
                data.get(i, data.rows_count() - 1) = sum - target_function[(i)];
            }
        }

        void removeNegativeB()
        {
            while(negativeBCheck())
            {
                auto maxBRow = -1;
                T maxB = 0;
                for(int i = 0; i < data.rows_count(); ++i)
                {
                    T val = data.get(data.columns_count() - 1, i);
                    if(val < 0 && !compare_float(maxB, val) && std::abs(maxB) < std::abs(val))
                    {
                        maxB = val;
                        maxBRow = i;
                    }
                }

                auto maxBCol = -1;
                T maxElemAtBRow = 0;
                bool allPositive = true;
                for(int i = 0; i < data.columns_count() - 1; ++i)
                {
                    T val = data.get(i, maxBRow);
                    if(val < 0)
                    {
                        allPositive = false;
                        if(!compare_float(maxElemAtBRow, val) && std::abs(maxElemAtBRow) < std::abs(val))
                        {
                            maxElemAtBRow = val;
                            maxBCol = i;
                        }
                    }
                }

                if(allPositive || maxBCol == -1 || maxBCol == -1) //throw?
                {
                    return;
                }

                auto resolvingElement = data.get(maxBCol, maxBRow);
                for(int i = 0; i < data.columns_count(); ++i)
                {
                    T a = data.get(i, maxBRow);
                    T res = a / resolvingElement;
                    data.get(i, maxBRow) = res;
                }

                for(int i = 0; i < data.rows_count(); ++i)
                {
                    if(i != maxBRow)
                    {
                        auto b = data.get(maxBCol, i);
                        for(int j = 0; j < data.columns_count(); ++j)
                        {
                            T a = data.get(j, maxBRow);
                            data.get(j, i) -= a * b;
                        }
                    }
                }
                orderCoefficients.insert_or_assign(maxBRow, maxBCol);
            }
        }

        bool negativeBCheck()
        {
            bool hasNegativeB = false;
            for(int i = 0; i < data.rows_count(); ++i)
            {
                T val = data.get(data.columns_count() - 1, i);
                if(!compare_float(val, 0.0) && val < 0)
                    hasNegativeB = true;
            }
            return hasNegativeB;
        }

        void basisFormation(const std::vector<size_t>& cols) //
        {
            const size_t bazisVarCount = data.rows_count();
            size_t row = 0;
            for(auto& col: cols)
            {
                if(auto bc = checkBazisAt(col); bc.second)
                {
                    orderCoefficients.insert_or_assign(row, col);
                } else
                {
                    findBazisAt(col, row);
                    orderCoefficients.insert_or_assign(row, col);
                }
                ++row;
            }
        }

        void findBazisAt(size_t col, size_t row)
        {
            auto resolvingElement = data.get(col, row);
            for(int i = 0; i < data.columns_count(); ++i)
            {
                auto a = data.get(i, row);
                auto res = a / resolvingElement;
                data.get(i, row) = res;
            }

            for(int i = 0; i < data.rows_count(); ++i)
            {
                if(i != row)
                {
                    auto b = data.get(col, i);
                    for(int j = 0; j < data.columns_count(); ++j)
                    {
                        auto a = data.get(j, row);
                        data.get(j, i) -= a * b;
                    }
                }
            }
        }

        auto checkBazisAt(size_t col) const
        {
            int zeroCount = 0;
            int index = 0;
            for(int i = 0; i < data.rows_count(); i++)
            {
                if(data.get(col, i) == 0)
                    zeroCount++;
                else
                    index = i;
            }
            return std::make_pair(index, zeroCount == data.rows_count() - 1);
        }

        bool positiveDCheck()
        {
            bool hasPositiveD = false;
            for(int i = 0; i < data.columns_count() - 1; ++i)
            {
                T val = data.get(i, data.rows_count() - 1);
                if(val > 0 && !compare_float(val, 0))
                    hasPositiveD = true;
            }
            return hasPositiveD;
        }

        void simplexMethod()
        {
            if(positiveDCheck())
            {

            }
        }

    public:
        SimplexMethod() = default;

        void init(const MatrixStorage<T>& data)
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            input = data;
            this->data = data.dropRow(1);
            _varCount = data.columns_count() - 1;
            _limitedCount = data.rows_count() - 1;
            for(int i = 0; i < input.columns_count() - 1; ++i)
                target_function.emplace_back(input.get(i, 0));
            makeCanonicalForm();
        }

        SimplexResult<T> solve() override
        {
            return SimplexResult<T>();
        }

    protected:
        std::string serialize() override
        {
            return std::string();
        }

        void deserialize(const std::string& string) override
        {

        }
    };
}

#endif //MAGPIE_SIMPLEXMETHOD_H
