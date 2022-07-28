
#include <gtest/gtest.h>
#include <Solver.h>

class TestSolver : public ::testing::Test
{
protected:
    TestSolver()
    {}

    virtual ~TestSolver()
    {}

    void SetUp(const std::vector<std::vector<double>>& vec)
    {
        Billet::MatrixStorage<double> dataMatrix(vec.size(), vec[1].size());
        for(int i = 0; i < vec.size(); ++i)
        {
            for(int j = 0; j < vec[0].size(); ++j)
            {
                dataMatrix.get(j, i) = vec[i][j];
            }
        }
        data = new Billet::GraphicMet2D<double>(dataMatrix, 2, 3);
    }

    virtual void TearDown()
    {
       // delete tc;
        delete data;
    }

    Billet::GraphicMet2D<double>* data;
    Billet::GraphicMet2D<double>* tc;
};

TEST_F(TestSolver, param1)
{
    SetUp({{-3, -2, 0, 0},
           {1,  2,  1, 7},
           {2,  1,  1, 8},
           {0,  1,  1, 3}});
    EXPECT_EQ(0, 0);
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}