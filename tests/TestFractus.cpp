#include <gtest/gtest.h>
#include "include/Fractus.h"

TEST(Fractus, FarctusTest_1)
{
    double d = 1.0/2;
    double d2 = 1.0/2;
    Magpie::Fractus f(1,2);
    Magpie::Fractus f2(1,2);

    ASSERT_EQ((f * f2).convertTO<double>(), d * d2);
    ASSERT_EQ((f / f2).convertTO<double>(), d / d2);
    ASSERT_EQ((f + f2).convertTO<double>(), d + d2);
    ASSERT_EQ((f - f2).convertTO<double>(), d - d2);
    ASSERT_EQ((-f2).convertTO<double>(), -d2);
}

TEST(Fractus, FarctusTest_2)
{
    double d = 1.0/2;
    double d2 = 1.0/2;
    double d3 = 3.0/2;
    Magpie::Fractus f(1,2);
    Magpie::Fractus f2(1,2);
    Magpie::Fractus f3(3,2);

    ASSERT_EQ((f * f2 - f3).convertTO<double>(), d * d2 - d3);
    ASSERT_EQ((f * f2 / f3).convertTO<double>(), d * d2 / d3);
    ASSERT_EQ((f * f2 + f3).convertTO<double>(), d * d2 + d3);
    ASSERT_EQ((f * (f2 - f3)).convertTO<double>(), d * (d2 - d3));
}