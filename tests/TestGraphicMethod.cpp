#include <Solver.h>
#include <gtest/gtest.h>

class TestSolver : public ::testing::Test
{
protected:
    TestSolver()
    {}

    virtual ~TestSolver()
    {}

    void SetUp(const std::vector<std::vector<double>>& vec)
    {
        Magpie::MatrixStorage<double> dataMatrix(vec.size(), vec[1].size());
        for(int i = 0; i < vec.size(); ++i)
        {
            for(int j = 0; j < vec[0].size(); ++j)
            {
                dataMatrix.get(j, i) = vec[i][j];
            }
        }
        data = new Magpie::GraphicMet2D<double>(dataMatrix, 2, 3);
    }

    virtual void TearDown()
    {
        // delete tc;
        delete data;
    }

    Magpie::GraphicMet2D<double>* data;
    Magpie::GraphicMet2D<double>* tc;
};

TEST_F(TestSolver, param1)
{
    SetUp({{-3, -2, 0, 0},
           {1,  2,  1, 7},
           {2,  1,  1, 8},
           {0,  1,  1, 3}});
    SetUp({{-3, -2, 0, 0},
           {2,  3,  -1, 8},
           {-8,  1,  1, 5},
           {8,  1,  1, 40}});
    EXPECT_EQ(0, 0);
}