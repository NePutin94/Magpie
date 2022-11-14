#include "include/Solver.h"
#include <gtest/gtest.h>

using typelist = testing::Types<double, float>;
template<class T>
struct TestGraphMetSort : testing::Test
{
};

TYPED_TEST_SUITE(TestGraphMetSort, typelist);
TYPED_TEST(TestGraphMetSort, TestGraphMetSort_Func1)
{
    std::vector<palka::Vec2<TypeParam>> order{palka::Vec2<TypeParam>{0.588, 0.412}, palka::Vec2<TypeParam>{0.948, 3.106}, palka::Vec2<TypeParam>{1.65, 1.7},
                                              palka::Vec2<TypeParam>{0.867, 0.133}};

    Magpie::GraphPrivateWrapper<TypeParam> test;
    auto res = test.call(
            {palka::Vec2<TypeParam>{0.588, 0.412}, palka::Vec2<TypeParam>{1.65, 1.7}, palka::Vec2<TypeParam>{0.948, 3.106},
             palka::Vec2<TypeParam>{0.867, 0.133}},

            {Magpie::Plot<TypeParam>{2, 1, 5, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<TypeParam>{-15, 2, -8, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<TypeParam>{-10, 5, -8, Magpie::Sign::GREATEROREQUAL},
             Magpie::Plot<TypeParam>{1, 1, 1, Magpie::Sign::GREATEROREQUAL}});

    ASSERT_EQ(order.size(), res.size());

    int index = -1;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] == res[0])
        {
            index = i;
            break;
        }
    }
    EXPECT_TRUE(index > -1);
    std::rotate(order.begin(),
                order.end() - (order.size() - index),
                order.end());

    bool allHit = true;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] != res[i])
        {
            allHit = false;
            break;
        }
    }

    EXPECT_TRUE(allHit);
}

TYPED_TEST_SUITE(TestGraphMetSort, typelist);
TYPED_TEST(TestGraphMetSort, TestGraphMetSort_Func2)
{
    std::vector<palka::Vec2<double>> order{palka::Vec2<double>{0.588, 0.412},
                                           palka::Vec2<double>{0.867, 0.133},
                                           palka::Vec2<double>{1.65, 1.7},
                                           palka::Vec2<double>{0.948, 3.106}
    };

    Magpie::GraphPrivateWrapper<double> test;
    auto res = test.call(
            {palka::Vec2<double>{0.588, 0.412}, palka::Vec2<double>{0.867, 0.133}, palka::Vec2<double>{0.948, 3.106}, palka::Vec2<double>{1.65, 1.7}},

            {Magpie::Plot<double>{2, 1, 5, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{-15, 2, -8, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{-10, 5, -8, Magpie::Sign::GREATEROREQUAL},
             Magpie::Plot<double>{1, 1, 1, Magpie::Sign::GREATEROREQUAL}});

    ASSERT_EQ(order.size(), res.size());

    int index = -1;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] == res[0])
        {
            index = i;
            break;
        }
    }
    EXPECT_TRUE(index > -1);
    std::rotate(order.begin(),
                order.end() - (order.size() - index),
                order.end());

    bool allHit = true;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] != res[i])
        {
            allHit = false;
            break;
        }
    }

    EXPECT_TRUE(allHit);
}

TYPED_TEST_SUITE(TestGraphMetSort, typelist);
TYPED_TEST(TestGraphMetSort, TestGraphMetSort_Func3)
{
    std::vector<palka::Vec2<double>> order{palka::Vec2<double>{0, 0},
                                           palka::Vec2<double>{0, 3.5},
                                           palka::Vec2<double>{0.667, 3.333},
                                           palka::Vec2<double>{1.833, 0.417},
                                           palka::Vec2<double>{1, 0}};

    Magpie::GraphPrivateWrapper<double> test;
    auto res = test.call(
            {palka::Vec2<double>{0, 0}, palka::Vec2<double>{0, 3.5}, palka::Vec2<double>{1, 0}, palka::Vec2<double>{0.667, 3.333},
             palka::Vec2<double>{1.833, 0.417}},

            {Magpie::Plot<double>{5, 2, 10, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{1, 4, 14, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{1, -2, 1, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{1, 0, 0, Magpie::Sign::GREATEROREQUAL},
             Magpie::Plot<double>{0, 1, 0, Magpie::Sign::GREATEROREQUAL}});

    ASSERT_EQ(order.size(), res.size());

    int index = -1;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] == res[0])
        {
            index = i;
            break;
        }
    }
    EXPECT_TRUE(index > -1);
    std::rotate(order.begin(),
                order.end() - (order.size() - index),
                order.end());

    bool allHit = true;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] != res[i])
        {
            allHit = false;
            break;
        }
    }

    EXPECT_TRUE(allHit);
}

TYPED_TEST_SUITE(TestGraphMetSort, typelist);
TYPED_TEST(TestGraphMetSort, TestGraphMetSort_Func4)
{
    std::vector<palka::Vec2<double>> order{
            palka::Vec2<double>{0, 3},
            palka::Vec2<double>{0, 1},
            palka::Vec2<double>{1, 0},
            palka::Vec2<double>{4.5, 1.75},
            palka::Vec2<double>{2, 3},
            palka::Vec2<double>{0.667, 3.333},
    };

    Magpie::GraphPrivateWrapper<double> test;
    auto res = test.call(
            {palka::Vec2<double>{1, 0}, palka::Vec2<double>{0, 3}, palka::Vec2<double>{2, 3},
             palka::Vec2<double>{0.667, 3.333}, palka::Vec2<double>{4.5, 1.75}, palka::Vec2<double>{0, 1}},

            {Magpie::Plot<double>{1, 2, 8, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{1, 4, 14, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{1, -2, 1, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{-2, 4, 12, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{1, 1, 1, Magpie::Sign::GREATEROREQUAL},
             Magpie::Plot<double>{1, 0, 0, Magpie::Sign::GREATEROREQUAL},
             Magpie::Plot<double>{0, 1, 0, Magpie::Sign::GREATEROREQUAL}});

    ASSERT_EQ(order.size(), res.size());

    int index = -1;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] == res[0])
        {
            index = i;
            break;
        }
    }
    EXPECT_TRUE(index > -1);
    std::rotate(order.begin(),
                order.end() - (order.size() - index),
                order.end());

    bool allHit = true;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] != res[i])
        {
            allHit = false;
            break;
        }
    }

    EXPECT_TRUE(allHit);
}

TYPED_TEST_SUITE(TestGraphMetSort, typelist);
TYPED_TEST(TestGraphMetSort, TestGraphMetSort_Func5)
{
    std::vector<palka::Vec2<double>> order{palka::Vec2<double>{3.588, 1.294},
                                           palka::Vec2<double>{3.467, 2.267},
                                           palka::Vec2<double>{2, 3},
                                           palka::Vec2<double>{0.667, 3.333},
                                           palka::Vec2<double>{0, 3},
                                           palka::Vec2<double>{0, 1},
                                           palka::Vec2<double>{1, 0}};


    Magpie::GraphPrivateWrapper<double> test;
    auto res = test.call(
            {palka::Vec2<double>{1, 0}, palka::Vec2<double>{0, 3}, palka::Vec2<double>{3.588, 1.294}, palka::Vec2<double>{2, 3},
             palka::Vec2<double>{0.667, 3.333}, palka::Vec2<double>{3.467, 2.267}, palka::Vec2<double>{0, 1}},

            {Magpie::Plot<double>{1, 2, 8, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{1, 4, 14, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{1, -2, 1, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{-2, 4, 12, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{1, 1, 1, Magpie::Sign::GREATEROREQUAL},
             Magpie::Plot<double>{8, 1, 30, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{1, 0, 0, Magpie::Sign::GREATEROREQUAL},
             Magpie::Plot<double>{0, 1, 0, Magpie::Sign::GREATEROREQUAL}});

    ASSERT_EQ(order.size(), res.size());

    int index = -1;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] == res[0])
        {
            index = i;
            break;
        }
    }
    EXPECT_TRUE(index > -1);
    std::rotate(order.begin(),
                order.end() - (order.size() - index),
                order.end());

    bool allHit = true;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] != res[i])
        {
            allHit = false;
            break;
        }
    }

    EXPECT_TRUE(allHit);
}

TYPED_TEST_SUITE(TestGraphMetSort, typelist);
TYPED_TEST(TestGraphMetSort, TestGraphMetSort_Func6) //Union with holes
{
    std::vector<palka::Vec2<double>> order{palka::Vec2<double>{3, 2},
                                           palka::Vec2<double>{0, 2},
                                           palka::Vec2<double>{0, 0.5},
                                           palka::Vec2<double>{0.5, 0.25},
                                           palka::Vec2<double>{1, 0.5}
    };


    Magpie::GraphPrivateWrapper<double> test;
    auto res = test.call(
            {palka::Vec2<double>{0, 2}, palka::Vec2<double>{3, 2}, palka::Vec2<double>{0.5, 0.25}, palka::Vec2<double>{1, 0.5},
             palka::Vec2<double>{0, 0.5}},

            {Magpie::Plot<double>{4, -1, -2, Magpie::Sign::GREATEROREQUAL},
             Magpie::Plot<double>{1, -2, 0, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{1, 2, 1, Magpie::Sign::GREATEROREQUAL},
             Magpie::Plot<double>{-3, 4, -1, Magpie::Sign::GREATEROREQUAL},
             Magpie::Plot<double>{-3, 3, -3, Magpie::Sign::GREATEROREQUAL},
             Magpie::Plot<double>{1, 0, 0, Magpie::Sign::GREATEROREQUAL},
             Magpie::Plot<double>{0, 1, 0, Magpie::Sign::GREATEROREQUAL}});

    ASSERT_EQ(order.size(), res.size());

    int index = -1;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] == res[0])
        {
            index = i;
            break;
        }
    }
    EXPECT_TRUE(index > -1);
    std::rotate(order.begin(),
                order.end() - (order.size() - index),
                order.end());

    bool allHit = true;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] != res[i])
        {
            allHit = false;
            break;
        }
    }

    EXPECT_TRUE(allHit);
}


TYPED_TEST_SUITE(TestGraphMetSort, typelist); //Check magic swap
TYPED_TEST(TestGraphMetSort, TestGraphMetSort_Func7)
{
    std::vector<palka::Vec2<double>> order{palka::Vec2<double>{3.588, 1.294},
                                           palka::Vec2<double>{1, 0},
                                           palka::Vec2<double>{0, 1},
                                           palka::Vec2<double>{0, 3},
                                           palka::Vec2<double>{0.667, 3.333},
                                           palka::Vec2<double>{2, 3},
                                           palka::Vec2<double>{3.467, 2.267}
    };


    Magpie::GraphPrivateWrapper<double> test;
    auto res = test.call(
            {palka::Vec2<double>{2, 3}, palka::Vec2<double>{3.467, 2.267}, palka::Vec2<double>{0.667, 3.333}, palka::Vec2<double>{0, 3},
             palka::Vec2<double>{3.588, 1.294}, palka::Vec2<double>{1, 0}, palka::Vec2<double>{0, 1}},

            {Magpie::Plot<double>{1, 2, 8, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{1, 4, 14, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{1, -2, 1, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{-2, 4, 12, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{1, 1, 1, Magpie::Sign::GREATEROREQUAL},
             Magpie::Plot<double>{8, 1, 30, Magpie::Sign::LESSOREQUAL},
             Magpie::Plot<double>{1, 0, 0, Magpie::Sign::GREATEROREQUAL},
             Magpie::Plot<double>{0, 1, 0, Magpie::Sign::GREATEROREQUAL}});

    ASSERT_EQ(order.size(), res.size());

    int index = -1;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] == res[0])
        {
            index = i;
            break;
        }
    }
    EXPECT_TRUE(index > -1);
    std::rotate(order.begin(),
                order.end() - (order.size() - index),
                order.end());

    bool allHit = true;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] != res[i])
        {
            allHit = false;
            break;
        }
    }

    EXPECT_TRUE(allHit);
}
