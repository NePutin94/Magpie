#include "Application.h"

using namespace Magpie;


int main()
{
//    std::vector<std::vector<double>> vec = {{-3, -2, 0, 0},
//                                            {1,  2,  1, 7},
//                                            {2,  1,  1, 8},
//                                            {0,  1,  1, 3}};
//    Billet::MatrixStorage<double> dataMatrix(vec.size(), vec[1].size());
//    for(int i = 0; i < vec.size(); ++i)
//    {
//        for(int j = 0; j < vec[0].size(); ++j)
//        {
//            dataMatrix.get(j, i) = vec[i][j];
//        }
//    }
//    Billet::GraphicMet2D<double> t(dataMatrix, 2,3);
//Y 54 50 2 false
//X 101 1 50 false

    Magpie::Application app({1280, 720});
    app.run();
    return 0;
}
