#include <gtest/gtest.h>
#include <Solver.h>

using typelist = testing::Types<int, double, float>;
template<class>
struct TestPlot : testing::Test
{};

TYPED_TEST_SUITE(TestPlot, typelist);
TYPED_TEST(TestPlot, TestIntersect_Func1)
{
    Magpie::Plot<TypeParam> l1{1, 1, 5, Magpie::Sign::EQUAL};
    Magpie::Plot<TypeParam> l2{-1, 1, 6, Magpie::Sign::EQUAL};
    auto res = l1.intersection(l2);
    if constexpr(std::is_integral_v<TypeParam>)
    {
        EXPECT_TRUE(res.x == 0 && res.y == 5); //-0.5 5.5 -> 0 5
    } else
        EXPECT_TRUE(res.x < 0 && res.y > 5); //-0.5 5.5

}

TYPED_TEST_SUITE(TestPlot, typelist);
TYPED_TEST(TestPlot, TestIntersect_Func2)
{
    Magpie::Plot<TypeParam> l1{1, 1, 5, Magpie::Sign::EQUAL};
    Magpie::Plot<TypeParam> l2{1, 1, 6, Magpie::Sign::EQUAL};
    auto res = l1.intersection(l2);
    EXPECT_TRUE(res.x == std::numeric_limits<TypeParam>::min() &&
                res.y == std::numeric_limits<TypeParam>::min());
}

TYPED_TEST_SUITE(TestPlot, typelist);
TYPED_TEST(TestPlot, TestIntersect_Func3)
{
    Magpie::Plot<TypeParam> l1{1, 1, 5, Magpie::Sign::EQUAL};
    Magpie::Plot<TypeParam> l2{-5, -3, 6, Magpie::Sign::EQUAL};
    auto res = l1.intersection(l2);
    EXPECT_TRUE(res.x < -9 && res.y > 14); // for int -10.5 15.5 -> -10 15
}