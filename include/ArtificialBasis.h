#ifndef MAGPIE_ARTIFICIALBASIS_H
#define MAGPIE_ARTIFICIALBASIS_H

#include "MatrixStorage.h"
#include "SolverMemento.h"
#include "GraphicsMethod2D.h"

namespace Magpie
{

    enum class ArtificialBasisState
    {
        WAIT_DATA = 0,
        CANONICAL_FORM,
        SIXPLEX_MET,
        SELECT_SUPPORT_ELEM,
        SELECT_ARTI_SUPPORT_ELEM,
        DONE_HAS_RESULT,
        DONE_CANT_SOLVE,
        BAZIS_FIND,
        BAZIS_FIND_DONE
    };

    enum class ArtificialBasisResultType
    {
        SIMPLEX_TABLE,
        TABLE,
        INPUT_DATA,
        SELECT_SUPPORT_ELEM,
        SELECT_ARTI_SUPPORT_ELEM,
        RESULT_TABLE,
        FIND_BAZIS_TABLE,
        FIND_BASIS_RESULT
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
        static inline int arti_basis_iters = -1;
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

        void updateState(ArtificialBasisState siState)
        {
            switch(siState)
            {
                case ArtificialBasisState::WAIT_DATA:
                    arti_basis_iters = 0;
                    break;
                case ArtificialBasisState::CANONICAL_FORM:
                    arti_basis_iters = 0;
                    answer_str = "canonical form";
                    break;
                case ArtificialBasisState::SIXPLEX_MET:
                    arti_basis_iters = 0;
                    answer_str = "simplex method iteration";
                    break;
                case ArtificialBasisState::SELECT_SUPPORT_ELEM:
                    arti_basis_iters = 0;
                    answer_str = "selection support elem";
                    break;
                case ArtificialBasisState::SELECT_ARTI_SUPPORT_ELEM:
                    arti_basis_iters++;
                    answer_str = "selection support elem";
                    break;
                case ArtificialBasisState::DONE_HAS_RESULT:
                    arti_basis_iters = 0;
                    answer_str = "Result";
                    break;
                case ArtificialBasisState::DONE_CANT_SOLVE:
                    arti_basis_iters = 0;
                    answer_str = "Can't solve";
                    break;
                case ArtificialBasisState::BAZIS_FIND:
                    answer_str = "find bazis";
                    break;
            }
        }

        std::vector<size_t> vars;
        ArtificialBasisState alg_state;
        ArtificialBasisResultType state;
        std::string answer_str;
        size_t resolving_row;
        size_t resolving_col;
        size_t deleted_var;
        size_t deleted_col;

        SimplexResultIterative3() = default;

        SimplexResultIterative3(MatrixStorage<T> before, MatrixStorage<T> after, ArtificialBasisResultType state, std::vector<size_t> basis,
                                ArtificialBasisState alg_state, std::vector<size_t> vars = {}, size_t deleted_var = -1, size_t deleted_col = -1)
                : after(after), before(before), state(state), basis(basis), alg_state(alg_state), deleted_var(deleted_var), vars(vars), deleted_col(deleted_col)
        { updateState(alg_state); }

        SimplexResultIterative3(MatrixStorage<T> before, MatrixStorage<T> after, ArtificialBasisResultType state, size_t resolving_row, size_t resolving_col,
                                std::vector<size_t> basis, ArtificialBasisState alg_state, std::vector<size_t> vars = {}, size_t deleted_var = -1,
                                size_t deleted_col = -1)
                : after(after), before(before), state(state), resolving_col(resolving_col), resolving_row(resolving_row), basis(basis), alg_state(alg_state),
                  deleted_var(deleted_var), vars(vars), deleted_col(deleted_col)
        { updateState(alg_state); }

        SimplexResultIterative3(MatrixStorage<T> before, MatrixStorage<T> after, ArtificialBasisResultType state, std::map<size_t, size_t> possibleSupportElems,
                                std::pair<size_t, size_t> bestSupportElem, std::vector<size_t> basis, ArtificialBasisState alg_state,
                                std::vector<size_t> vars = {}, size_t deleted_var = -1, size_t deleted_col = -1)
                : after(after), before(before), state(state), possibleSupportElems(possibleSupportElems), bestSupportElem(bestSupportElem), basis(basis),
                  alg_state(alg_state), deleted_var(deleted_var), vars(vars), deleted_col(deleted_col)
        { updateState(alg_state); }
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
        size_t deleted_col;
        size_t deleted_var;
        size_t vars_count;
        size_t limit_count;
        ArtificialBasisState alg_state;
        std::pair<size_t, size_t> curr_support_elem;
        std::pair<size_t, size_t> curr_support_artib_elem;
    public:
        ArtificialBasis()
        {

        }

        ArtificialBasis(const MatrixStorage<T>& matrix) : data(matrix)
        {

        }

        SimplexResultIterative3<T> solve_iterative()
        {
            auto before = data;
            auto prev_state = alg_state;
            switch(alg_state)
            {
                case ArtificialBasisState::BAZIS_FIND_DONE:
                {
                    prepare_simplex();
                    return SimplexResultIterative3<T>(before, data, ArtificialBasisResultType::FIND_BASIS_RESULT, basis, prev_state, vars, deleted_var,
                                                      deleted_col);
                }
                case ArtificialBasisState::BAZIS_FIND:
                {
                    find_basis_iterative();
                    return SimplexResultIterative3<T>(before, data, ArtificialBasisResultType::FIND_BAZIS_TABLE, basis, prev_state, vars, deleted_var,
                                                      deleted_col);
                }
                case ArtificialBasisState::DONE_CANT_SOLVE:
                {
                    return SimplexResultIterative3<T>(before, data, ArtificialBasisResultType::TABLE, basis, prev_state, vars);
                }
                case ArtificialBasisState::DONE_HAS_RESULT:
                {
                    return SimplexResultIterative3<T>(before, data, ArtificialBasisResultType::RESULT_TABLE, basis, prev_state, vars);
                }
                case ArtificialBasisState::CANONICAL_FORM:
                {
                    makeCanonicalForm();
                    return SimplexResultIterative3<T>(before, data, ArtificialBasisResultType::TABLE, basis, prev_state, vars);
                }
                case ArtificialBasisState::SIXPLEX_MET:
                {
                    simplex_iterative();
                    return SimplexResultIterative3<T>(before, data, ArtificialBasisResultType::SIMPLEX_TABLE, curr_support_elem.first, curr_support_elem.second,
                                                      basis,
                                                      prev_state, vars);
                }
                case ArtificialBasisState::SELECT_SUPPORT_ELEM:
                {
                    auto elems = possibleSupportElements();
                    return SimplexResultIterative3<T>(before, data, ArtificialBasisResultType::SELECT_SUPPORT_ELEM, elems.second, elems.first, basis,
                                                      prev_state, vars);
                }
                case ArtificialBasisState::SELECT_ARTI_SUPPORT_ELEM:
                {
                    auto elems = possibleSupportElements2();
                    return SimplexResultIterative3<T>(before, data, ArtificialBasisResultType::SELECT_ARTI_SUPPORT_ELEM, elems.second, elems.first, basis,
                                                      prev_state, vars);
                }
                case ArtificialBasisState::WAIT_DATA:
                    break;
            }
            return SimplexResultIterative3<T>();
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
                        auto res = (!compare_float<T>(b, 0) && !compare_float<T>(a, 0)) ? (a / b) : std::numeric_limits<int>::max();
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
                alg_state = ArtificialBasisState::DONE_CANT_SOLVE;
            }
            return std::make_pair(std::make_pair(minBRow, maxDeltaCol), ret);
        }

        bool positiveDeltaCheck()
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            bool allPositive = true;
            for(int i = 0; i < data.columns_count() - 1; ++i)
            {
                if(data.get(i, data.rows_count() - 1) < (T) 0)
                {
                    allPositive = false;
                    break;
                }
            }
            return allPositive;
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
                    alg_state = ArtificialBasisState::DONE_CANT_SOLVE;
                    return;
                }

                const auto old_data = data;
                const auto resolvingElement = data.get(maxDeltaCol, minBRow);
                std::swap(basis[minBRow], vars[maxDeltaCol]);
                //basis[minBRow] = maxDeltaCol + 1;
                //dividing the row by the resolving element
                for(int i = 0; i < data.columns_count(); ++i)
                {
                    if(i == maxDeltaCol) //resolving = 1/resolving
                        data.get(i, minBRow) = (T) 1 / data.get(i, minBRow);
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
                if(!positiveDeltaCheck())
                    alg_state = ArtificialBasisState::SELECT_SUPPORT_ELEM;
                else
                    alg_state = ArtificialBasisState::DONE_HAS_RESULT;
            } else
                alg_state = ArtificialBasisState::DONE_HAS_RESULT;
        }

        SimplexResult3<T> solve() override
        {
            return SimplexResult3<T>();
        }

        SimplexResultIterative3<T> init(const MatrixStorage<T>& data)
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
            alg_state = ArtificialBasisState::CANONICAL_FORM;
            return SimplexResultIterative3<T>(input, input, ArtificialBasisResultType::INPUT_DATA, basis, ArtificialBasisState::WAIT_DATA);
        }

        bool negativeBCheck()
        {
            bool hasNegativeB = false;
            for(int i = 0; i < data.rows_count(); ++i)
            {
                T val = data.get(data.columns_count() - 1, i);
                if(!compare_float<T>(val, 0.0) && val < (T) 0)
                    hasNegativeB = true;
            }
            return hasNegativeB;
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
                    if(val < 0 && !compare_float<T>(maxB, val) && abs(maxB) < abs(val))
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
                        if(!compare_float<T>(maxElemAtBRow, val) && abs(maxElemAtBRow) < abs(val))
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
            }
        }

        void makeCanonicalForm()
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            MatrixStorage<T> result;
            std::map<int, int> idsNewVar;
            for(int i = 0; i < data.rows_count(); ++i) //search for rows with inequality signs
                if(auto sig = (Sign) (int) data.get(data.columns_count() - 2, i); sig != Sign::EQUAL)
                    idsNewVar.emplace(i, (sig == Sign::LESSOREQUAL) ? 1 : -1);

            data = data.dropColumn(data.columns_count() - 2); //inequality signs are no longer needed

            if(!idsNewVar.empty())
            {
                result.alloc_matrix(data.rows_count() + 2, data.columns_count() + idsNewVar.size()); //a matrix with space for new variables and deltas
                vars_count += idsNewVar.size();
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
                data.row_realloc(data.rows_count() + 2);
            }
            for(int j = 0; j < data.columns_count(); ++j)
            {
                if(j < target_function.size())
                    data.get(j, data.rows_count() - 1) = target_function[j];
                else
                    data.get(j, data.rows_count() - 1) = 0;
            }
            removeNegativeB();
            prepare_basis_find();
            alg_state = ArtificialBasisState::SELECT_ARTI_SUPPORT_ELEM;
        }

        void prepare_basis_find()
        {
            for(int i = 0; i < data.columns_count(); ++i)
            {
                T sum = 0;
                for(int j = 0; j < data.rows_count() - 2; ++j)
                    sum += data.get(i, j);
                data.get(i, data.rows_count() - 2) = -sum;
            }
            for(int i = 0; i < data.rows_count() - 2; ++i) //the last row is not a limitation
                basis.emplace_back(data.columns_count() + i);

            for(int i = 0; i < data.columns_count() - 1; ++i)
                vars.emplace_back(i + 1);
        }

        bool isSupportVariable(size_t var_id)
        {
            return var_id > vars_count;
        }

        bool check()
        {
            if(data.get(data.columns_count() - 1, data.rows_count() - 2) == (T) 0)
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

        auto possibleSupportElements2()
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            std::map<size_t, size_t> ret;
            auto maxDelta = std::numeric_limits<T>::max() * -1;
            auto maxDeltaCol = -1;

            auto minBDiv = std::numeric_limits<T>::max();
            auto minBRow = -1;

            bool allNullOrPositive = true;
            for(int j = 0; j < basis.size(); ++j)
            {
                if(!isSupportVariable(basis[j])) //we take only artificial variables??
                    continue;
                else
                    allNullOrPositive = false;
                for(int i = 0; i < data.columns_count() - 1; ++i)
                {
                    if(data.get(i, data.rows_count() - 2) >= (T) 0)
                        continue;
                    if(data.get(i, j) > (T) 0)
                    {
                        maxDelta = data.get(i, j);
                        maxDeltaCol = i;
                        for(int r = 0; r < data.rows_count() - 2; ++r) //checking that a row with an artificial variable has min{a/b}
                        {
                            auto a = data.get(data.columns_count() - 1, r);
                            auto b = data.get(maxDeltaCol, r);
                            if(b < 0)
                                continue;
                            T res = (!compare_float<T>(b, 0) && !compare_float<T>(a, 0)) ? (a / b) : std::numeric_limits<T>::max();
                            if(res < minBDiv)
                            {
                                minBDiv = res;
                                minBRow = r;
                            }
                        }
                        if(minBRow == j)
                        {
                            ret.emplace(i, j);
                        } else
                            minBDiv = std::numeric_limits<T>::max();
                    }
                }
            }
            if(allNullOrPositive)
                alg_state = ArtificialBasisState::BAZIS_FIND_DONE;
            return std::make_pair(std::make_pair(minBRow, maxDeltaCol), ret);
        }

        void prepare_simplex()
        {
            data = data.dropRow(data.rows_count() - 2);
            if(!positiveDeltaCheck())
                alg_state = ArtificialBasisState::SELECT_SUPPORT_ELEM;
            else
                alg_state = ArtificialBasisState::DONE_HAS_RESULT;
        }

        void find_basis_iterative()
        {
            if(!check())
            {
                auto maxDeltaCol = curr_support_artib_elem.second;
                auto minBRow = curr_support_artib_elem.first;

                if(maxDeltaCol == -1 || minBRow == -1) //something's wrong.
                {
                    ArtificialBasisState::DONE_CANT_SOLVE;
                    return;
                }

                const auto old_data = data;
                const auto resolvingElement = data.get(maxDeltaCol, minBRow);

                deleted_col = maxDeltaCol;
                deleted_var = basis[minBRow];
                basis[minBRow] = vars[maxDeltaCol];
                vars.erase(vars.begin() + maxDeltaCol);
                for(int i = 0; i < data.columns_count(); ++i)
                {
                    if(i == maxDeltaCol)
                        data.get(i, minBRow) = (T) 0 / data.get(i, minBRow);
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
                if(check())
                    alg_state = ArtificialBasisState::BAZIS_FIND_DONE;
                else
                    alg_state = ArtificialBasisState::SELECT_ARTI_SUPPORT_ELEM;
            } else
            {
                alg_state = ArtificialBasisState::SELECT_SUPPORT_ELEM;
            }
        }

        void find_basis()
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
                    if(auto new_val = data.get(i, data.rows_count() - 2);
                            data.get(i, data.rows_count() - 2) < (T) 0)
                    {
                        maxDelta = new_val;
                        maxDeltaCol = i;
                        for(int j = 0; j < basis.size(); ++j)
                            if(isSupportVariable(basis[j]) &&
                               data.get(maxDeltaCol, j) > (T) 0) //let's take some basic support variable and check that the value
                                //in this cell is greater than zero - so in theory
                                //we can take this variable as the base one
                                //which is guaranteed to replace the artificial basic one
                            {
                                for(int r = 0; r < data.rows_count() - 2; ++r)
                                {
                                    auto a = data.get(data.columns_count() - 1, r);
                                    auto b = data.get(maxDeltaCol, r);
                                    if(b < 0)
                                        continue;
                                    T res = (!compare_float<T>(b, 0) && !compare_float<T>(a, 0)) ? (a / b) : std::numeric_limits<T>::max();
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

                auto test = possibleSupportElements2();

                if(maxDeltaCol == -1 || minBRow == -1) //something's wrong.
                {
                    return;
                }

                const auto old_data = data;
                const auto resolvingElement = data.get(maxDeltaCol, minBRow);

                deleted_col = maxDeltaCol;
                basis[minBRow] = vars[maxDeltaCol];
                vars.erase(vars.begin() + maxDeltaCol);
                for(int i = 0; i < data.columns_count(); ++i)
                {
                    if(i == maxDeltaCol)
                        data.get(i, minBRow) = (T) 0 / data.get(i, minBRow);
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
                alg_state = ArtificialBasisState::BAZIS_FIND;
            } else
            {
                alg_state = ArtificialBasisState::SIXPLEX_MET;
            }
        }

        void setSupportElem(size_t row, size_t col)
        {
            curr_support_elem = {row, col};
            alg_state = ArtificialBasisState::SIXPLEX_MET;
        }

        void setSupportArtiElem(size_t row, size_t col)
        {
            curr_support_artib_elem = {row, col};
            alg_state = ArtificialBasisState::BAZIS_FIND;
        }

        std::string serialize() override
        {
            nlohmann::json json;
            auto& inputLayout = json["Input"];
            for(int i = 0; i < input.columns_count() - 1; ++i)
                inputLayout["TargetFunc"].emplace_back(input.get(i, 0));

            for(int i = 1; i < input.rows_count(); ++i)
            {
                auto& lim = inputLayout["Limitations"][fmt::format("Lim_{}", i)];
                for(int j = 0; j < input.columns_count();++j)
                    lim.emplace_back(input.get(j, i));
            }
            return json.dump(4);
        }

        void deserialize(const std::string& string) override
        {

        }

        ArtificialBasis<T> createMemento() const override
        {
            return *this;
        }

        bool reinstateMemento(const ArtificialBasis<T>& mem) override
        {
            *this = mem;
            return true;
        }
    };

} // Magpie

#endif //MAGPIE_ARTIFICIALBASIS_H
