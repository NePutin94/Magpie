#include "include/Solver.h"
#include <gtest/gtest.h>
#include "include/MatrixStorage.h"

using typelist = testing::Types<double, float>;
template<class T>
struct TestGraphMetSort : testing::Test
{
};

template<class T>
inline void fill(Magpie::MatrixStorage<T>& storage, int var)
{
    switch(var)
    {
        case 0:
            storage.alloc_matrix(3, 6);
            storage.get(0, 0) = -2;
            storage.get(1, 0) = -1;
            storage.get(2, 0) = -3;
            storage.get(3, 0) = -1;

            storage.get(0, 1) = 1;
            storage.get(1, 1) = 2;
            storage.get(2, 1) = 5;
            storage.get(3, 1) = -1;
            storage.get(4, 1) = 0;
            storage.get(5, 1) = 4;

            storage.get(0, 2) = 1;
            storage.get(1, 2) = -1;
            storage.get(2, 2) = -1;
            storage.get(3, 2) = 2;
            storage.get(4, 2) = 0;
            storage.get(5, 2) = 1;
            break;
        case 1:
            storage.alloc_matrix(4, 4);
            storage.get(0, 0) = -3;
            storage.get(1, 0) = -2;

            storage.get(0, 1) = 1;
            storage.get(1, 1) = 2;
            storage.get(2, 1) = 1;
            storage.get(3, 1) = 7;

            storage.get(0, 2) = 2;
            storage.get(1, 2) = 1;
            storage.get(2, 2) = 1;
            storage.get(3, 2) = 8;

            storage.get(0, 3) = 0;
            storage.get(1, 3) = 1;
            storage.get(2, 3) = 1;
            storage.get(3, 3) = 3;
            break;
        case 2:
            storage.alloc_matrix(4, 4);
            storage.get(0, 0) = -1;
            storage.get(1, 0) = -2;

            storage.get(0, 1) = 2;
            storage.get(1, 1) = -1;
            storage.get(2, 1) = -1;
            storage.get(3, 1) = -1;

            storage.get(0, 2) = 1;
            storage.get(1, 2) = -2;
            storage.get(2, 2) = 1;
            storage.get(3, 2) = 0;

            storage.get(0, 3) = 1;
            storage.get(1, 3) = 1;
            storage.get(2, 3) = -1;
            storage.get(3, 3) = 1;
        case 3:
            storage.alloc_matrix(5, 4);
            storage.get(0, 0) = -1;
            storage.get(1, 0) = -2;

            storage.get(0, 1) = 2;
            storage.get(1, 1) = 1;
            storage.get(2, 1) = 1;
            storage.get(3, 1) = 5;

            storage.get(0, 2) = 1;
            storage.get(1, 2) = 2;
            storage.get(2, 2) = 1;
            storage.get(3, 2) = 8;

            storage.get(0, 3) = 5;
            storage.get(1, 3) = 1;
            storage.get(2, 3) = 1;
            storage.get(3, 3) = 9;

            storage.get(0, 4) = 6;
            storage.get(1, 4) = -1;
            storage.get(2, 4) = 1;
            storage.get(3, 4) = 9;
            break;
        case 4:
            storage.alloc_matrix(6, 4);
            storage.get(0, 0) = -1;
            storage.get(1, 0) = -2;

            storage.get(0, 1) = 2;
            storage.get(1, 1) = 1;
            storage.get(2, 1) = Magpie::Sign::LESSOREQUAL;
            storage.get(3, 1) = 5;

            storage.get(0, 2) = 1;
            storage.get(1, 2) = 2;
            storage.get(2, 2) = Magpie::Sign::LESSOREQUAL;
            storage.get(3, 2) = 8;

            storage.get(0, 3) = 1;
            storage.get(1, 3) = 1;
            storage.get(2, 3) = Magpie::Sign::GREATEROREQUAL;
            storage.get(3, 3) = 1;

            storage.get(0, 4) = -15;
            storage.get(1, 4) = 2;
            storage.get(2, 4) = Magpie::Sign::LESSOREQUAL;
            storage.get(3, 4) = -8;

            storage.get(0, 5) = -10;
            storage.get(1, 5) = 5;
            storage.get(2, 5) = Magpie::Sign::GREATEROREQUAL;
            storage.get(3, 5) = -8;
            break;
        case 5:
            storage.alloc_matrix(6, 4);
            storage.get(0, 0) = -1;
            storage.get(1, 0) = -2;

            storage.get(0, 1) = 4;
            storage.get(1, 1) = -1;
            storage.get(2, 1) = Magpie::Sign::GREATEROREQUAL;
            storage.get(3, 1) = -2;

            storage.get(0, 2) = 1;
            storage.get(1, 2) = -2;
            storage.get(2, 2) = Magpie::Sign::LESSOREQUAL;
            storage.get(3, 2) = 0;

            storage.get(0, 3) = 1;
            storage.get(1, 3) = 2;
            storage.get(2, 3) = Magpie::Sign::GREATEROREQUAL;
            storage.get(3, 3) = 1;

            storage.get(0, 4) = -3;
            storage.get(1, 4) = 4;
            storage.get(2, 4) = Magpie::Sign::GREATEROREQUAL;
            storage.get(3, 4) = -1;

            storage.get(0, 5) = -3;
            storage.get(1, 5) = 3;
            storage.get(2, 5) = Magpie::Sign::GREATEROREQUAL;
            storage.get(3, 5) = -3;
            break;
        case 6:
            storage.alloc_matrix(7, 4);
            storage.get(0, 0) = -1;
            storage.get(1, 0) = -2;

            storage.get(0, 1) = 1;
            storage.get(1, 1) = 2;
            storage.get(2, 1) = Magpie::Sign::LESSOREQUAL;
            storage.get(3, 1) = 8;

            storage.get(0, 2) = 1;
            storage.get(1, 2) = 4;
            storage.get(2, 2) = Magpie::Sign::LESSOREQUAL;
            storage.get(3, 2) = 14;

            storage.get(0, 3) = 1;
            storage.get(1, 3) = -2;
            storage.get(2, 3) = Magpie::Sign::LESSOREQUAL;
            storage.get(3, 3) = 1;

            storage.get(0, 4) = -2;
            storage.get(1, 4) = 4;
            storage.get(2, 4) = Magpie::Sign::LESSOREQUAL;
            storage.get(3, 4) = 12;

            storage.get(0, 5) = 1;
            storage.get(1, 5) = 1;
            storage.get(2, 5) = Magpie::Sign::GREATEROREQUAL;
            storage.get(3, 5) = 1;

            storage.get(0, 6) = 8;
            storage.get(1, 6) = 1;
            storage.get(2, 6) = Magpie::Sign::LESSOREQUAL;
            storage.get(3, 6) = 30;
            break;
    }
}

TYPED_TEST_SUITE(TestGraphMetSort, typelist);
TYPED_TEST(TestGraphMetSort, TestGraphMetSort_Func1)
{
    Magpie::MatrixStorage<float> storage(0, 0);
    Magpie::GraphicMet2D<float> solver;
    fill(storage, 0);
    solver.init(storage, storage.columns_count() - 2, storage.rows_count() - 1);
}