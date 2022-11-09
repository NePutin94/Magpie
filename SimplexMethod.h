#ifndef MAGPIE_SIMPLEXMETHOD_H
#define MAGPIE_SIMPLEXMETHOD_H

#include "ISolver.h"
#include "MatrixStorage.h"
#include "SolverMemento.h"
#include <tracy/Tracy.hpp>

namespace Magpie
{
    enum class SimxpleMetState
    {
        WAIT_DATA = 0,
        REMOVE_NEG_COEFF,
        CALC_DELTA,
        SIXPLEX_MET,
        DONE_HAS_RESULT,
        DONE_CANT_SOLVE,
        BAZIS_FIND
    };

    enum class SimxpleMetState2
    {
        WAIT_DATA = 0,
        CANONICAL_FORM,
        SIXPLEX_MET,
        SELECT_SUPPORT_ELEM,
        DONE_HAS_RESULT,
        DONE_CANT_SOLVE,
        BAZIS_FIND
    };

    enum class SiMetResultType
    {
        SIMPLEX_TABLE,
        TABLE,
        INPUT_DATA,
        SELECT_SUPPORT_ELEM
    };

    constexpr std::string SimxpleMetState2String(SimxpleMetState state)
    {
        std::string_view arr[] = {"wait_data", "remove_neg_coeff", "calc_delta", "simplex_met", "done_has_result", "done_cant_solve", "bazis_find"};
        return arr[(int) state].data();
    }

    template<class T>
    class SimplexResult
    {
    };

    template<class T>
    class SimplexResultIterative2 : IMementoData<SimplexResultIterative2<T>>
    {
    public:
        MatrixStorage<T> before;
        MatrixStorage<T> after;
        std::map<size_t, size_t> possibleSupportElems;
        std::pair<size_t, size_t> bestSupportElem;
        std::vector<size_t> basis;
    public:
        SimplexResultIterative2<T> createMemento() const override
        {
            return *this;
        }

        bool reinstateMemento(const SimplexResultIterative2<T>& mem) override
        {
            *this = mem;
            return true;
        }

        SiMetResultType state;
        size_t resolving_row;
        size_t resolving_col;

        SimplexResultIterative2() = default;

        SimplexResultIterative2(MatrixStorage<T> before, MatrixStorage<T> after, SiMetResultType state, std::vector<size_t> basis) : after(after),
                                                                                                                                     before(before),
                                                                                                                                     state(state), basis(basis)
        {}

        SimplexResultIterative2(MatrixStorage<T> before, MatrixStorage<T> after, SiMetResultType state, size_t resolving_row, size_t resolving_col,
                                std::vector<size_t> basis) : after(after), before(before), state(state),
                                                             resolving_col(resolving_col), resolving_row(resolving_row), basis(basis)
        {}

        SimplexResultIterative2(MatrixStorage<T> before, MatrixStorage<T> after, SiMetResultType state, std::map<size_t, size_t> possibleSupportElems,
                                std::pair<size_t, size_t> bestSupportElem, std::vector<size_t> basis)
                : after(after), before(before), state(state), possibleSupportElems(possibleSupportElems), bestSupportElem(bestSupportElem), basis(basis)
        {}
    };

    template<class T>
    class SimplexResultIterative
    {
    private:
        struct _marker
        {
            int x;
            int y;
            std::string message;
        };

        MatrixStorage<T> before;
        MatrixStorage<T> after;

    public:
        SimxpleMetState state;

        SimplexResultIterative() = default;

        SimplexResultIterative(MatrixStorage<T> before, MatrixStorage<T> after, SimxpleMetState state) : after(after), before(before), state(state)
        {}
    };

    template<class T>
    inline bool compare_float(T x, T y, T epsilon = 0.001f)
    {
        if(fabs(x - y) < epsilon)
            return true;
        return false;
    }

    template<class T>
    class SimplexMethod2 : public Solver<T, SimplexResult<T>>, IMementoData<SimplexMethod2<T>>
    {
    public:
        size_t vars_count;
        size_t limit_count;
        SimxpleMetState2 alg_state;
        MatrixStorage<T> input;
        MatrixStorage<T> data; //all solve function
        std::vector<T> target_function;
        std::vector<size_t> basis;
        std::vector<size_t> init_basis;
        std::pair<size_t, size_t> curr_support_elem;

        SimplexMethod2() : alg_state(SimxpleMetState2::WAIT_DATA)
        {

        }

        SimplexResult<T> solve() override
        {
            return SimplexResult<T>();
        }

        SimplexMethod2<T> createMemento() const override
        {
            return *this;
        }

        bool reinstateMemento(const SimplexMethod2<T>& mem) override
        {
            *this = mem;
            return true;
        }

        void setSupportElem(size_t row, size_t col)
        {
            curr_support_elem = {row, col};
            alg_state = SimxpleMetState2::SIXPLEX_MET;
        }

        SimplexResultIterative2<T> solve_iterative()
        {
            auto before = data;
            switch(alg_state)
            {
                case SimxpleMetState2::BAZIS_FIND:
                {
                    basisFormation(init_basis);
                    return SimplexResultIterative2<T>(before, data, SiMetResultType::TABLE, basis);
                }
                case SimxpleMetState2::DONE_CANT_SOLVE:
                {
                    return SimplexResultIterative2<T>(before, data, SiMetResultType::TABLE, basis);
                }
                case SimxpleMetState2::CANONICAL_FORM:
                {
                    makeCanonicalForm();
                    return SimplexResultIterative2<T>(before, data, SiMetResultType::TABLE, basis);
                }
                case SimxpleMetState2::SIXPLEX_MET:
                {
                    simplex_iterative();
                    return SimplexResultIterative2<T>(before, data, SiMetResultType::SIMPLEX_TABLE, curr_support_elem.first, curr_support_elem.second, basis);
                }
                case SimxpleMetState2::SELECT_SUPPORT_ELEM:
                {
                    auto elems = possibleSupportElements();
                    return SimplexResultIterative2<T>(before, data, SiMetResultType::SELECT_SUPPORT_ELEM, elems.second, elems.first, basis);
                }
                case SimxpleMetState2::WAIT_DATA:
                    break;
            }
            return SimplexResultIterative2<T>();
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
            alg_state = SimxpleMetState2::BAZIS_FIND;
        }

        void basisFormation(const std::vector<size_t>& cols)
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            std::set<size_t> toDrop;
            size_t row = 0;
            for(auto& col: cols)
            {
                if(auto bc = checkBazisAt(col); bc.second)
                {
                    toDrop.emplace(col);
                    basis.emplace_back(col + 1);
                } else
                {
                    toDrop.emplace(col);
                    findBazisAt(col, row);
                    basis.emplace_back(col + 1);
                }
                ++row;
            }
            data = data.dropColumns(toDrop);
            alg_state = SimxpleMetState2::SELECT_SUPPORT_ELEM;
        }

        void findBazisAt(size_t col, size_t row)
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
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

            //applying a transformation to an objective function
            auto b = data.get(col, data.rows_count() - 1);
            for(int j = 0; j < data.columns_count(); ++j)
            {
                auto a = data.get(j, row);
                data.get(j, data.rows_count() - 1) -= a * b;
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

        auto possibleSupportElements()
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            bool allNegative = true;
            std::map<size_t, size_t> ret;

            auto maxDelta = std::numeric_limits<T>::max() * -1;
            auto maxDeltaCol = -1;

            auto minBDiv = std::numeric_limits<T>::max();
            auto minBRow = -1;

            for(int i = 0; i < data.columns_count() - 1; ++i)
            {
                bool new_maxDelta = false;
                if(data.get(i, data.rows_count() - 1) < 0)
                {
                    if(auto new_val = data.get(i, data.rows_count() - 1); maxDelta < new_val)
                    {
                        maxDelta = new_val;
                        maxDeltaCol = i;
                        new_maxDelta = true;
                    }
                    auto min = std::numeric_limits<T>::max();
                    minBDiv = min;
                    auto row = -1;
                    for(int j = 0; j < data.rows_count() - 1; ++j)
                    {
                        auto a = data.get(data.columns_count() - 1, j);
                        auto b = data.get(i, j);
                        if(b < 0)
                            continue;

                        allNegative = false;
                        auto res = (!compare_float(b, 0.0) && !compare_float(a, 0.0)) ? (a / b) : std::numeric_limits<int>::max();
                        if(res < min)
                        {
                            min = res;
                            row = j;
                            if(new_maxDelta)
                            {
                                minBDiv = res;
                                minBRow = j;
                            }
                        }
                    }
                    new_maxDelta = false;
                    ret[i] = row;
                }
            }
            if(allNegative) //can't be solve -inf
            {
                alg_state = SimxpleMetState2::DONE_CANT_SOLVE;
            }
            return std::make_pair(std::make_pair(minBRow, maxDeltaCol), ret);
        }

        void simplex_iterative()
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            if(!positiveDeltaCheck())
            {
//                auto maxDelta = std::numeric_limits<T>::max() * -1;
//                auto maxDeltaCol = -1;
//                for(int i = 0; i < data.columns_count() - 1; ++i)
//                {
//                    if(auto new_val = data.get(i, data.rows_count() - 1);
//                            data.get(i, data.rows_count() - 1) < 0 && maxDelta < new_val)
//                    {
//                        maxDelta = new_val;
//                        maxDeltaCol = i;
//                    }
//                }
//
//                bool allNegative = true;
//                auto minBDiv = std::numeric_limits<T>::max();
//                auto minBRow = -1;
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
                auto maxDeltaCol = curr_support_elem.second;
                auto minBRow = curr_support_elem.first;
//                if(allNegative) //can't be solve -inf
//                {
//                    alg_state = SimxpleMetState2::DONE_CANT_SOLVE;
//                    return;
//                }

                if(maxDeltaCol == -1 || minBRow == -1) //something's wrong.
                {
                    alg_state = SimxpleMetState2::DONE_CANT_SOLVE;
                    return;
                }

                const auto old_data = data;
                const auto resolvingElement = data.get(maxDeltaCol, minBRow);
                basis[minBRow] = maxDeltaCol + 1;
                //dividing the row by the resolving element
                for(int i = 0; i < data.columns_count(); ++i)
                {
                    if(i == maxDeltaCol) //resolving = 1/resolving
                        data.get(i, minBRow) = 1.0 / data.get(i, minBRow);
                    else //the rest are divided into a resolving element
                        data.get(i, minBRow) /= resolvingElement;
                }

                //dividing the col by the resolving element and multiplying by -1
                for(int i = 0; i < data.rows_count(); ++i)
                {
                    if(i == minBRow) //nothing for the resolving element
                        continue;
                    else //divide and multiply by -1
                        data.get(maxDeltaCol, i) /= resolvingElement * -1;
                }

                //the remaining parts of the simplex table
                for(int i = 0; i < data.rows_count(); ++i)
                {
                    if(i == minBRow)
                        continue;
                    auto b = old_data.get(maxDeltaCol, i); //we take the old value of the resolving column
                    for(int j = 0; j < data.columns_count(); ++j)
                    {
                        if(j == maxDeltaCol)
                            continue;
                        data.get(j, i) -= data.get(j, minBRow) * b;
                    }
                }
                alg_state = SimxpleMetState2::SELECT_SUPPORT_ELEM;
            } else
                alg_state = SimxpleMetState2::DONE_HAS_RESULT;
        }

        bool positiveDeltaCheck()
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            bool allPositive = true;
            for(int i = 0; i < data.columns_count() - 1; ++i)
            {
                if(data.get(i, data.rows_count() - 1) < 0)
                {
                    allPositive = false;
                    break;
                }
            }
            return allPositive;
        }

        SimplexResultIterative2<T> init(const MatrixStorage<T>& data, std::vector<size_t> basis = {})
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            input = data;
            this->data = data.dropRow(0);
            for(int i = 0; i < input.columns_count() - 1; ++i)
                target_function.emplace_back(input.get(i, 0));
            vars_count = input.columns_count() - 2;
            limit_count = input.rows_count() - 2;
            init_basis = basis;
            alg_state = SimxpleMetState2::CANONICAL_FORM;
            return SimplexResultIterative2<T>(input, input, SiMetResultType::INPUT_DATA, basis);
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

    template<class T>
    class SimplexMethod : public Solver<T, SimplexResult<T>>, IMementoData<SimplexMethod<T>>
    {
    public:
        SimxpleMetState alg_state;
        MatrixStorage<T> input;
        MatrixStorage<T> data; //all solve function
        std::vector<int> bazisVars;
        std::vector<T> target_function;
        int _varCount;
        int _limitedCount;
        std::map<int, int> orderCoefficients;

        SimplexMethod<T> createMemento() const override
        {
            return *this;
        }

        bool reinstateMemento(const SimplexMethod<T>& mem) override
        {
            *this = mem;
            return true;
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

                for(int i = 0; i < idsNewVar.size(); ++i)
                    bazisVars.emplace_back(data.columns_count() - 1 + i);
                for(int i = bazisVars.size(), j = 0; i < data.rows_count(); ++i)
                    bazisVars.emplace_back(j++);
            }
            data = result;

            for(int j = 0; j < idsNewVar.size(); ++j)
                target_function.emplace_back(0);

            alg_state = SimxpleMetState::BAZIS_FIND;
        }

        void deltasCalculation()
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
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
            alg_state = SimxpleMetState::SIXPLEX_MET;
        }

        void removeNegativeB()
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            if(negativeBCheck())
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
                    alg_state = SimxpleMetState::DONE_CANT_SOLVE;
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
            } else
            {
                alg_state = SimxpleMetState::CALC_DELTA;
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

        void basisFormation(const std::vector<int>& cols)
        {
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
            alg_state = SimxpleMetState::REMOVE_NEG_COEFF;
        }

        void findBazisAt(size_t col, size_t row)
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
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
#ifdef TracyProfiler
            ZoneScoped;
#endif
            bool hasPositiveD = false;
            for(int i = 0; i < data.columns_count() - 1; ++i)
            {
                T val = data.get(i, data.rows_count() - 1);
                if(val > 0 && !compare_float(val, 0.0))
                    hasPositiveD = true;
            }
            return hasPositiveD;
        }

        void simplexMethod()
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            if(positiveDCheck())
            {
                T maxDelta = 0;
                auto maxDeltaCol = -1;
                for(int i = 0; i < data.columns_count() - 1; ++i)
                {
                    T val = data.get(i, data.rows_count() - 1);
                    if(val > 0 && !compare_float(maxDelta, val) && maxDelta < val)
                    {
                        maxDelta = val;
                        maxDeltaCol = i;
                    }
                }

                bool allNegative = true;

                T minBDiv = std::numeric_limits<int>::max();
                auto minBRow = -1;
                for(int i = 0; i < _limitedCount; ++i)
                {
                    T a = data.get(data.columns_count() - 1, i);
                    T b = data.get(maxDeltaCol, i);
                    if(b > 0)
                        allNegative = false;
                    T res = (b > 0 && !compare_float(b, 0.0) && !compare_float(a, 0.0)) ? (a / b) : std::numeric_limits<int>::max();
                    if(res < minBDiv)
                    {
                        minBDiv = res;
                        minBRow = i;
                    }
                }

                if(allNegative) //can't be solve -inf
                {
                    alg_state = SimxpleMetState::DONE_CANT_SOLVE;
                    return;
                }

                if(maxDeltaCol == -1 || minBRow == -1) //something's wrong.
                {
                    alg_state = SimxpleMetState::DONE_CANT_SOLVE;
                    return;
                }

                auto resolvingElement = data.get(maxDeltaCol, minBRow);
                for(int i = 0; i < data.columns_count(); ++i)
                {
                    T a = data.get(i, minBRow);
                    T res = a / resolvingElement;
                    data.get(i, minBRow) = res;
                }

                for(int i = 0; i < data.rows_count() - 1; ++i)
                {
                    if(i != minBRow)
                    {
                        auto b = data.get(maxDeltaCol, i);
                        for(int j = 0; j < data.columns_count(); ++j)
                        {
                            T a = data.get(j, minBRow);
                            data.get(j, i) -= a * b;
                        }
                    }
                }
                orderCoefficients.insert_or_assign(minBRow, maxDeltaCol);
                alg_state = SimxpleMetState::CALC_DELTA;
            } else
            {
                alg_state = SimxpleMetState::DONE_HAS_RESULT;
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
            this->data = data.dropRow(0);
            _varCount = data.columns_count() - 1;
            _limitedCount = data.rows_count() - 1;
            for(int i = 0; i < input.columns_count() - 1; ++i)
                target_function.emplace_back(input.get(i, 0));
            makeCanonicalForm();
        }

        SimplexResult<T> solve() override
        {
            return {};
        }

        SimplexResultIterative<T> solve_iterative()
        {
            auto before = data;
            switch(alg_state)
            {
                case SimxpleMetState::BAZIS_FIND:
                {
                    basisFormation(bazisVars);
                    return SimplexResultIterative<T>(before, data, alg_state);
                }
                case SimxpleMetState::CALC_DELTA:
                {
                    deltasCalculation();
                    return SimplexResultIterative<T>(before, data, alg_state);
                }
                case SimxpleMetState::DONE_CANT_SOLVE:
                {
                }
                    break;
                case SimxpleMetState::SIXPLEX_MET:
                {
                    simplexMethod();
                    return SimplexResultIterative<T>(before, data, alg_state);
                }
                case SimxpleMetState::WAIT_DATA:
                    break;
                case SimxpleMetState::REMOVE_NEG_COEFF:
                {
                    removeNegativeB();
                    return SimplexResultIterative<T>(before, data, alg_state);
                }
            }
            return SimplexResultIterative<T>();
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
