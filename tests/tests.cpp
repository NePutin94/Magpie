
#include <gtest/gtest.h>
#include <Application.h>

class TheClassTest : public ::testing::Test
{
protected:
    TheClassTest()
    {}

    virtual ~TheClassTest()
    {}

    void SetUp(double a, double b, double c)
    {
        data = new Billet::Plot2<double>(a, b, c);
        tc = new Billet::Plot2<double>(*data);
    }

    virtual void TearDown()
    {
        delete tc;
        delete data;
    }

    Billet::Plot2<double>* data;
    Billet::Plot2<double>* tc;
};

TEST_F(TheClassTest, param1)
{
    SetUp(1, 2, 3);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}