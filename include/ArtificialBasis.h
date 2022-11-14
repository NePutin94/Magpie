//
// Created by dimka on 10/22/2022.
//

#ifndef MAGPIE_ARTIFICIALBASIS_H
#define MAGPIE_ARTIFICIALBASIS_H

#include "MatrixStorage.h"
#include "SolverMemento.h"
#include "Solver.h"

namespace Magpie
{

    enum class ArtificialBasisState
    {
        WAIT_DATA = 0,
        CANONICAL_FORM,
        SIXPLEX_MET,
        SELECT_SUPPORT_ELEM,
        DONE_HAS_RESULT,
        DONE_CANT_SOLVE,
        BAZIS_FIND
    };

    enum class ArtificialBasisResultType
    {
        SIMPLEX_TABLE,
        TABLE,
        INPUT_DATA,
        SELECT_SUPPORT_ELEM
    };

    template<class T>
    class SimplexResult3
    {
    };

    template<class T>
    class SimplexResultIterative3 : IMementoData<SimplexResultIterative3<T>>
    {
    public:
        MatrixStorage<T> before;
        MatrixStorage<T> after;
        std::map<size_t, size_t> possibleSupportElems;
        std::pair<size_t, size_t> bestSupportElem;
        std::vector<size_t> basis;
    public:
        SimplexResultIterative3<T> createMemento() const override
        {
            return *this;
        }

        bool reinstateMemento(const SimplexResultIterative3<T>& mem) override
        {
            *this = mem;
            return true;
        }

        ArtificialBasisResultType state;
        size_t resolving_row;
        size_t resolving_col;

        SimplexResultIterative3() = default;

        SimplexResultIterative3(MatrixStorage<T> before, MatrixStorage<T> after, ArtificialBasisResultType state, std::vector<size_t> basis) : after(after),
                                                                                                                                               before(before),
                                                                                                                                               state(state),
                                                                                                                                               basis(basis)
        {}

        SimplexResultIterative3(MatrixStorage<T> before, MatrixStorage<T> after, ArtificialBasisResultType state, size_t resolving_row, size_t resolving_col,
                                std::vector<size_t> basis) : after(after), before(before), state(state),
                                                             resolving_col(resolving_col), resolving_row(resolving_row), basis(basis)
        {}

        SimplexResultIterative3(MatrixStorage<T> before, MatrixStorage<T> after, ArtificialBasisResultType state, std::map<size_t, size_t> possibleSupportElems,
                                std::pair<size_t, size_t> bestSupportElem, std::vector<size_t> basis)
                : after(after), before(before), state(state), possibleSupportElems(possibleSupportElems), bestSupportElem(bestSupportElem), basis(basis)
        {}
    };

    template<class T>
    class ArtificialBasis : public Solver<T, SimplexResult3<T>>, IMementoData<ArtificialBasis<T>>
    {
    private:
        MatrixStorage<T> data;
        std::vector<size_t> basis;
        std::vector<size_t> vars;
        MatrixStorage<T> input;
        std::vector<T> target_function;
        size_t vars_count;
        size_t limit_count;
    public:
        ArtificialBasis()
        {

        }

        ArtificialBasis(const MatrixStorage<T>& matrix) : data(matrix)
        {

        }

        SimplexResult3<T> solve() override
        {
            return SimplexResult3<T>();
        }

        void init(const MatrixStorage<T>& data, std::vector<size_t> basis = {})
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            input = data;
            vars_count = input.columns_count() - 2;
            limit_count = input.rows_count() - 2;
            this->data = data.dropRow(0);
            for(int i = 0; i < input.columns_count() - 1; ++i)
                target_function.emplace_back(input.get(i, 0));
            makeCanonicalForm();
        }

        void makeCanonicalForm()
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            MatrixStorage<T> result;
            std::map<int, int> idsNewVar;
            for(int i = 0; i < data.rows_count(); ++i) //search for rows with inequality signs
                if(auto sig = (Sign) data.get(data.columns_count() - 2, i); sig != Sign::EQUAL)
                    idsNewVar.emplace(i, (sig == Sign::LESSOREQUAL) ? 1 : -1);

            data = data.dropColumn(data.columns_count() - 2); //inequality signs are no longer needed

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
                            result.get(data.columns_count() + idsNewVar.size() - 1, i) = data.get(j, i) * newVar.second;
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
                data = result;
            } else
            {
                data.row_realloc(data.rows_count() + 1);
            }
            for(int j = 0; j < data.columns_count(); ++j)
            {
                data.get(j, data.rows_count() - 1) = target_function[j];
            }
            prepare();
        }

        void prepare()
        {
            for(int i = 0; i < data.columns_count(); ++i)
            {
                T sum = 0;
                for(int j = 0; j < data.rows_count() - 1; ++j)
                    sum += data.get(i, j);
                data.get(i, data.rows_count() - 1) = -sum;
            }
            for(int i = 0; i < data.rows_count() - 1; ++i) //the last row is not a limitation
                basis.emplace_back(data.columns_count() + i);

            for(int i = 0; i < data.columns_count() - 1; ++i)
                vars.emplace_back(i);
        }

        bool isSupportVariable(size_t var_id)
        {
            return var_id > vars_count;
        }

        bool check()
        {
            if(data.get(data.columns_count() - 1, data.rows_count() - 1) == 0)
            {
                bool allNotSupport = true;
                for(auto& id: basis)
                    if(isSupportVariable(id))
                    {
                        allNotSupport = false;
                        break;
                    }
                return allNotSupport;
            }
            return false;
        }

        void simplex_method()
        {
            if(!check())
            {

                auto maxDelta = std::numeric_limits<T>::max() * -1;
                auto maxDeltaCol = -1;

                auto minBDiv = std::numeric_limits<T>::max();
                auto minBRow = -1;

                bool resolvingFinded = false;

                for(int i = 0; i < data.columns_count() - 1; ++i)
                {
                    if(resolvingFinded)
                        break;
                    if(auto new_val = data.get(i, data.rows_count() - 1);
                            data.get(i, data.rows_count() - 1) < 0)
                    {
                        maxDelta = new_val;
                        maxDeltaCol = i;
                        for(int j = 0; j < basis.size(); ++j)
                            if(isSupportVariable(basis[j]) && data.get(maxDeltaCol, j) > 0) //let's take some basic support variable and check that the value
                                //in this cell is greater than zero - so in theory
                                //we can take this variable as the base one
                                //which is guaranteed to replace the artificial basic one
                            {
                                for(int r = 0; r < data.rows_count() - 1; ++r)
                                {
                                    auto a = data.get(data.columns_count() - 1, r);
                                    auto b = data.get(maxDeltaCol, r);
                                    if(b < 0)
                                        continue;
                                    T res = (!compare_float(b, 0.0) && !compare_float(a, 0.0)) ? (a / b) : std::numeric_limits<int>::max();
                                    if(res < minBDiv)
                                    {
                                        minBDiv = res;
                                        minBRow = r;
                                    }
                                }
                                if(minBRow == j)
                                {
                                    resolvingFinded = true;
                                    break;
                                } else
                                    minBDiv = std::numeric_limits<T>::max();
                            }
                    }
                }

//                bool allNegative = true;
//
//                for(int i = 0; i < data.rows_count() - 1; ++i)
//                {
//                    auto a = data.get(data.columns_count() - 1, i);
//                    auto b = data.get(maxDeltaCol, i);
//                    if(b < 0)
//                        continue;
//
//                    allNegative = false;
//                    T res = (!compare_float(b, 0.0) && !compare_float(a, 0.0)) ? (a / b) : std::numeric_limits<int>::max();
//                    if(res < minBDiv)
//                    {
//                        minBDiv = res;
//                        minBRow = i;
//                    }
//                }

//                if(allNegative) //can't be solve -inf
//                {
//                    return;
//                }

                if(maxDeltaCol == -1 || minBRow == -1) //something's wrong.
                {
                    return;
                }

                //simplex method: see SimplexMethod::simplex_iterative
                const auto old_data = data;
                const auto resolvingElement = data.get(maxDeltaCol, minBRow);

                basis[minBRow] = vars[maxDeltaCol] + 1;
                vars.erase(vars.begin() + maxDeltaCol);
                for(int i = 0; i < data.columns_count(); ++i)
                {
                    if(i == maxDeltaCol)
                        data.get(i, minBRow) = 1.0 / data.get(i, minBRow);
                    else
                        data.get(i, minBRow) /= resolvingElement;
                }

                for(int i = 0; i < data.rows_count(); ++i)
                {
                    if(i == minBRow)
                        continue;
                    else
                        data.get(maxDeltaCol, i) /= resolvingElement * -1;
                }

                for(int i = 0; i < data.rows_count(); ++i)
                {
                    if(i == minBRow)
                        continue;
                    auto b = old_data.get(maxDeltaCol, i);
                    for(int j = 0; j < data.columns_count(); ++j)
                    {
                        if(j == maxDeltaCol)
                            continue;
                        data.get(j, i) -= data.get(j, minBRow) * b;
                    }
                }

                data = data.dropColumn(maxDeltaCol);
            }
        }

    protected:
        std::string serialize() override
        {
            return std::string();
        }

        void deserialize(const std::string& string) override
        {

        }

    private:
        ArtificialBasis<T> createMemento() const override
        {
            return ArtificialBasis<T>();
        }

        bool reinstateMemento(const ArtificialBasis<T>& mem) override
        {
            return false;
        }

    };

} // Magpie

#endif //MAGPIE_ARTIFICIALBASIS_H
