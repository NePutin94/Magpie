#include <gtest/gtest.h>
#include "include/Solver.h"

using typelist = testing::Types<int, double, float>;
template<class>
struct TestPlot : testing::Test
{};

TYPED_TEST_SUITE(TestPlot, typelist);
TYPED_TEST(TestPlot, TestHasPoint_Func1)
{
    Magpie::Plot<TypeParam> test{1, 2, 7, Magpie::Sign::EQUAL};

    for(auto i = -50; i < 50; i += 1)
    {
        auto Xtest = test.getValueAtX(i);
        auto Ytest = test.getValueAtY(i);
        ASSERT_TRUE(test.hasPoint({Xtest, i})) << "X " << Xtest << " " << i << " Value:" << test.a * Xtest + test.b * i << " Test:"
                                               << Magpie::PlotCompare(test.a * Xtest + test.b * i, test.c);
        ASSERT_TRUE(test.hasPoint({i, Ytest})) << "Y " << i << " " << Ytest << " Value:" << test.a * i + test.b * Ytest << " Test:"
                                               << Magpie::PlotCompare(test.a * i + test.b * Ytest, test.c);
    }
}

TYPED_TEST_SUITE(TestPlot, typelist);
TYPED_TEST(TestPlot, TestHasPoint_Func2)
{
    Magpie::Plot<TypeParam> test{0, 1, 3, Magpie::Sign::EQUAL};

    for(auto i = -50; i < 50; i += 1)
    {
        auto Ytest = test.getValueAtY(i);
        ASSERT_TRUE(test.hasPoint({i, Ytest})) << "Y " << i << " " << Ytest << " Value:" << test.a * i + test.b * Ytest << " Test:"
                                               << Magpie::PlotCompare(test.a * i + test.b * Ytest, test.c);
    }
}

TYPED_TEST_SUITE(TestPlot, typelist);
TYPED_TEST(TestPlot, TestHasPoint_Func3)
{
    Magpie::Plot<TypeParam> test{2, 1, 8, Magpie::Sign::EQUAL};

    for(auto i = -50; i < 50; i += 1)
    {
        auto Xtest = test.getValueAtX(i);
        auto Ytest = test.getValueAtY(i);
        ASSERT_TRUE(test.hasPoint({Xtest, i})) << "X " << Xtest << " " << i << " Value:" << test.a * Xtest + test.b * i << " Test:"
                                               << Magpie::PlotCompare(test.a * Xtest + test.b * i, test.c);
        ASSERT_TRUE(test.hasPoint({i, Ytest})) << "Y " << i << " " << Ytest << " Value:" << test.a * i + test.b * Ytest << " Test:"
                                               << Magpie::PlotCompare(test.a * i + test.b * Ytest, test.c);
    }
}
