#include <Solver.h>
#include <gtest/gtest.h>

using typelist = testing::Types<double, float>;
template<class T>
struct TestGraphMetSort : testing::Test
{
};

TYPED_TEST_SUITE(TestGraphMetSort, typelist);
TYPED_TEST(TestGraphMetSort, TestGraphMetSort2_Func1)
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
    std::vector<std::pair<palka::Vec2<double>, std::vector<Magpie::Plot<double>>>> vec = {
            std::make_pair(palka::Vec2<double>{2, 3}, std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{1, 2, 8, Magpie::Sign::LESSOREQUAL},
                                                                                        Magpie::Plot<double>{1, 4, 14, Magpie::Sign::LESSOREQUAL}}),
            std::make_pair(palka::Vec2<double>{3.467, 2.267}, std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{1, 2, 8, Magpie::Sign::LESSOREQUAL},
                                                                                                Magpie::Plot<double>{8, 1, 30, Magpie::Sign::LESSOREQUAL}}),
            std::make_pair(palka::Vec2<double>{0.667, 3.333}, std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{1, 4, 14, Magpie::Sign::LESSOREQUAL},
                                                                                                Magpie::Plot<double>{-2, 4, 12, Magpie::Sign::LESSOREQUAL}}),
            std::make_pair(palka::Vec2<double>{0, 3}, std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{-2, 4, 12, Magpie::Sign::LESSOREQUAL},
                                                                                        Magpie::Plot<double>{1, 0, 0, Magpie::Sign::GREATEROREQUAL}}),
            std::make_pair(palka::Vec2<double>{3.588, 1.294}, std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{8, 1, 30, Magpie::Sign::LESSOREQUAL},
                                                                                                Magpie::Plot<double>{1, -2, 1, Magpie::Sign::LESSOREQUAL}}),
            std::make_pair(palka::Vec2<double>{1, 0}, std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{1, -2, 1, Magpie::Sign::LESSOREQUAL},
                                                                                        Magpie::Plot<double>{1, 1, 1, Magpie::Sign::GREATEROREQUAL}}),
            std::make_pair(palka::Vec2<double>{0, 1}, std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{1, 1, 1, Magpie::Sign::GREATEROREQUAL},
                                                                                        Magpie::Plot<double>{1, 0, 0, Magpie::Sign::GREATEROREQUAL}})};
    auto res = test.call2(vec);


    ASSERT_EQ(order.size(), res.size());
    int index = -1;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] == res[0].first)
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
        if(!Magpie::PlotCompare(order[i].x, res[i].first.x) || !Magpie::PlotCompare(order[i].y, res[i].first.y))
        {
            allHit = false;
            break;
        }
    }
    EXPECT_TRUE(allHit);
}

TYPED_TEST_SUITE(TestGraphMetSort, typelist);
TYPED_TEST(TestGraphMetSort, TestGraphMetSort2_Func2)
{
    std::vector<palka::Vec2<double>> order{palka::Vec2<double>{3.588, 1.294},
                                           palka::Vec2<double>{3.467, 2.267},
                                           palka::Vec2<double>{2, 3},
                                           palka::Vec2<double>{0.667, 3.333},
                                           palka::Vec2<double>{0, 3},
                                           palka::Vec2<double>{0, 1},
                                           palka::Vec2<double>{1, 0}};

    Magpie::GraphPrivateWrapper<double> test;
    std::vector<std::pair<palka::Vec2<double>, std::vector<Magpie::Plot<double>>>> vec = {
            std::make_pair(palka::Vec2<double>{1, 0}, std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{1, -2, 1, Magpie::Sign::LESSOREQUAL},
                                                                                        Magpie::Plot<double>{1, 1, 1, Magpie::Sign::GREATEROREQUAL}}),
            std::make_pair(palka::Vec2<double>{0, 3}, std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{-2, 4, 12, Magpie::Sign::LESSOREQUAL},
                                                                                        Magpie::Plot<double>{1, 0, 0, Magpie::Sign::GREATEROREQUAL}}),
            std::make_pair(palka::Vec2<double>{3.588, 1.294}, std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{8, 1, 30, Magpie::Sign::LESSOREQUAL},
                                                                                                Magpie::Plot<double>{1, -2, 1, Magpie::Sign::LESSOREQUAL}}),
            std::make_pair(palka::Vec2<double>{2, 3}, std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{1, 2, 8, Magpie::Sign::LESSOREQUAL},
                                                                                        Magpie::Plot<double>{1, 4, 14, Magpie::Sign::LESSOREQUAL}}),
            std::make_pair(palka::Vec2<double>{0.667, 3.333}, std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{1, 4, 14, Magpie::Sign::LESSOREQUAL},
                                                                                                Magpie::Plot<double>{-2, 4, 12, Magpie::Sign::LESSOREQUAL}}),
            std::make_pair(palka::Vec2<double>{3.467, 2.267}, std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{1, 2, 8, Magpie::Sign::LESSOREQUAL},
                                                                                                Magpie::Plot<double>{8, 1, 30, Magpie::Sign::LESSOREQUAL}}),
            std::make_pair(palka::Vec2<double>{0, 1}, std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{1, 1, 1, Magpie::Sign::GREATEROREQUAL},
                                                                                        Magpie::Plot<double>{1, 0, 0, Magpie::Sign::GREATEROREQUAL}})};

    auto res = test.call2(vec);
    ASSERT_EQ(order.size(), res.size());
    int index = -1;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] == res[0].first)
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
        if(!Magpie::PlotCompare(order[i].x, res[i].first.x) || !Magpie::PlotCompare(order[i].y, res[i].first.y))
        {
            allHit = false;
            break;
        }
    }
    EXPECT_TRUE(allHit);
}

TYPED_TEST_SUITE(TestGraphMetSort, typelist);
TYPED_TEST(TestGraphMetSort, TestGraphMetSort3_Func2)
{
    std::vector<palka::Vec2<double>> order{palka::Vec2<double>{0.948, 3.106}, palka::Vec2<double>{0.588, 0.412}, palka::Vec2<double>{0.867, 0.133},
                                           palka::Vec2<double>{1.65, 1.7}};
    Magpie::GraphPrivateWrapper<double> test;
    std::vector<std::pair<palka::Vec2<double>, std::vector<Magpie::Plot<double>>>> vec = {
            std::make_pair(palka::Vec2<double>{0.588, 0.412}, std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{1, 1, 1, Magpie::Sign::GREATEROREQUAL},
                                                                                                Magpie::Plot<double>{-15, 2, -8, Magpie::Sign::LESSOREQUAL}}),
            std::make_pair(palka::Vec2<double>{1.65, 1.7},
                           std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{-10, 5, -8, Magpie::Sign::GREATEROREQUAL},
                                                             Magpie::Plot<double>{2, 1, 5, Magpie::Sign::LESSOREQUAL}}),
            std::make_pair(palka::Vec2<TypeParam>{0.867, 0.133},
                           std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{-10, 5, -8, Magpie::Sign::GREATEROREQUAL},
                                                             Magpie::Plot<double>{1, 1, 1, Magpie::Sign::GREATEROREQUAL}}),
            std::make_pair(palka::Vec2<TypeParam>{0.948, 3.106},
                           std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{-15, 2, -8, Magpie::Sign::LESSOREQUAL},
                                                             Magpie::Plot<double>{2, 1, 5, Magpie::Sign::LESSOREQUAL}})};

    auto res = test.call2(vec);
    ASSERT_EQ(order.size(), res.size());
    int index = -1;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] == res[0].first)
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
        if(!Magpie::PlotCompare(order[i].x, res[i].first.x) || !Magpie::PlotCompare(order[i].y, res[i].first.y))
        {
            allHit = false;
            break;
        }
    }
    EXPECT_TRUE(allHit);
}

TYPED_TEST_SUITE(TestGraphMetSort, typelist);
TYPED_TEST(TestGraphMetSort, TestGraphMetSort3_Func3)
{

    std::vector<palka::Vec2<double>> order{palka::Vec2<double>{18, 10},
                                           palka::Vec2<double>{10, 2},
                                           palka::Vec2<double>{7, 0.5},
                                           palka::Vec2<double>{5, 0},
                                           palka::Vec2<double>{3, 0},
                                           palka::Vec2<double>{-1, 8},
                                           palka::Vec2<double>{-1.0667, 8.4}
                                           };
    Magpie::GraphPrivateWrapper<double> test;
    std::vector<std::pair<palka::Vec2<double>, std::vector<Magpie::Plot<double>>>> vec = {
            std::make_pair(palka::Vec2<double>{5, 0}, std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{1, -4, 5, Magpie::Sign::LESSOREQUAL},
                                                                                                Magpie::Plot<double>{0, 1, 0, Magpie::Sign::GREATEROREQUAL}}),
            std::make_pair(palka::Vec2<double>{-1, 8},
                           std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{6, 1, 2, Magpie::Sign::GREATEROREQUAL},
                                                             Magpie::Plot<double>{2, 1, 6, Magpie::Sign::GREATEROREQUAL}}),
            std::make_pair(palka::Vec2<double>{10, 2},
                           std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{1, -2, 6, Magpie::Sign::LESSOREQUAL},
                                                             Magpie::Plot<double>{1, -1, 8, Magpie::Sign::LESSOREQUAL}}),
            std::make_pair(palka::Vec2<double>{3, 0},
                           std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{2, 1, 6, Magpie::Sign::GREATEROREQUAL},
                                                             Magpie::Plot<double>{0, 1, 0, Magpie::Sign::GREATEROREQUAL}}),
            std::make_pair(palka::Vec2<double>{7, 0.5},
                           std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{1, -2, 6, Magpie::Sign::LESSOREQUAL},
                                                             Magpie::Plot<double>{1, -4, 5, Magpie::Sign::LESSOREQUAL}}),
            std::make_pair(palka::Vec2<double>{18, 10},
                           std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{1, -1, 8, Magpie::Sign::LESSOREQUAL},
                                                             Magpie::Plot<double>{-2, 1, -26, Magpie::Sign::GREATEROREQUAL}}),
            std::make_pair(palka::Vec2<double>{-1.0667, 8.4},
                            std::vector<Magpie::Plot<double>>{Magpie::Plot<double>{-9, 1, 18, Magpie::Sign::LESSOREQUAL},
                                                              Magpie::Plot<double>{6, 1, 2, Magpie::Sign::GREATEROREQUAL}})};

    auto res = test.call2(vec);
    ASSERT_EQ(order.size(), res.size());
    int index = -1;
    for(int i = 0; i < order.size(); ++i)
    {
        if(order[i] == res[0].first)
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
        if(!Magpie::PlotCompare(order[i].x, res[i].first.x) || !Magpie::PlotCompare(order[i].y, res[i].first.y))
        {
            allHit = false;
            break;
        }
    }
    EXPECT_TRUE(allHit);
}
