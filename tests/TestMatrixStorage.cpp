#include <gtest/gtest.h>
#include "MatrixStorage.h"
#include <random>

using typelist = testing::Types<int, double, float>;
template<class>
struct TestMatrix : testing::Test
{
};

TYPED_TEST_SUITE(TestMatrix, typelist);
TYPED_TEST(TestMatrix, TestMatrix_Func1)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(-5000, 5000);
    std::vector<TypeParam> vec;

    for(int i = 0; i < 100; ++i)
        vec.emplace_back(dist6(rng));

    for(int i = 1; i <= 20; ++i)
    {
        Magpie::MatrixStorage<TypeParam> test(vec, ceil(vec.size() / i), i);

        for(int col = 0; col < test.columns_count(); ++col) //first col equal
            ASSERT_EQ(test.get(col, 0), vec[col]);

        for(int row = 0; row < test.rows_count(); ++row)
            for(int col = 0; col < test.columns_count(); ++col)//matrix diagonal
            {
                if(row == col)
                    ASSERT_EQ(test.get(col, col), vec[row * i + col]);
            }
    }
}

TYPED_TEST_SUITE(TestMatrix, typelist);
TYPED_TEST(TestMatrix, TestMatrix_Func2)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist6(-5000, 5000);
    std::vector<TypeParam> vec;

    for(int i = 0; i < 20; ++i)
        vec.emplace_back(dist6(rng));

    for(int i = 1; i <= 5; ++i)
    {
        Magpie::MatrixStorage<TypeParam> test(vec, ceil(vec.size() / i), i);

        auto testDropRow = test.dropRow(1);
        ASSERT_EQ(testDropRow.rows_count(), test.rows_count() - 1);
        for(int col = 0; col < test.columns_count(); ++col)
            ASSERT_EQ(testDropRow.get(col, 1), test.get(col, 2)) << test.rows_count() << "-" << test.columns_count() << " | " << i << "-" << col;
    }
}

