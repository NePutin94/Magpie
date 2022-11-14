//
// Created by dimka on 10/9/2022.
//

#ifndef MAGPIE_DATASETS_H
#define MAGPIE_DATASETS_H

#include <vector>
#include "MatrixStorage.h"

namespace Magpie
{
    inline auto set1()//14
    {
        std::vector<double> inn = {-1, 8, 3, 0, 0,
                                   3, 1, 2, -1, 6,
                                   1, 1, 1, -1, 4,
                                   -1, 3, -1, -1, 4};
        return MatrixStorage < double > (inn, 4, 5);
    }

    inline auto set2()//-3
    {
        std::vector<double> inn = {-1, 2, 0, 0,
                                   1, -1, 1, 3,
                                   3, -4, -1, -12,
                                   1, 0, 1, 5};
        return MatrixStorage < double > (inn, 4, 4);
    }

    inline auto set3() //-223
    {
        std::vector<double> inn = {-7, -3, -5, 0, 0,
                                   1, 1, 1, 1, 50,
                                   1, 2, 3, 1, 73,
                                   3, 2, 1, 1, 75};
        return MatrixStorage < double > (inn, 4, 5);
    }

    inline auto set4() //-3
    {
        std::vector<double> inn = {1, -1, 0, 0,
                                   -2, 1, 1, 2,
                                   1, 1, 1, 5,
                                   1, -2, 1, 12};
        return MatrixStorage < double > (inn, 4, 4);
    }

    inline auto set5() //17
    {
        std::vector<double> inn = {3, 2, 1, 0, 0,
                                   0, 1, -1, -1, 4,
                                   2, 1, 2, -1, 6,
                                   2, -1, 2, -1, 2};
        return MatrixStorage < double > (inn, 4, 5);
    }

    inline auto set6() //-4
    {
        std::vector<double> inn = {-1, -1, -1, 0, 0,
                                   -1, 1, 1, 0, 2,
                                   3, -1, 1, 0, 0};
        return MatrixStorage < double > (inn, 3, 5);
    }

    inline auto set7() //-5
    {
        std::vector<Fractus> inn = {-2, -1, 3, 1, 0, 0,
                                   1, 2, 5, -1, 0, 4,
                                   1, -1, -1, 2, 0, 1};
        return MatrixStorage<Fractus> (inn, 3, 6);
    }
}
#endif //MAGPIE_DATASETS_H
