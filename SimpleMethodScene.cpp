#include "SimplexMethodScene.h"
#include "SimplexMethodView.h"
#include "DataSets.h"
void Magpie::SimplexMethodScene::init()
{
//    std::vector<double> inn = {-1, 2, 0, 0,
//                               1, -1, 1, 3,
//                               3, -4, -1, -12,
//                               1, 0, 1, 5};
    std::vector<double> inn = {-1, 8, 3, 0, 0,
                                3, 1, 2,-1, 6,
                                1, 1, 1,-1, 4,
                               -1, 3,-1,-1, 4};
    MatrixStorage<double> in(inn, 4, 5);
    SimplexMethodView s(set7());
    addView<SimplexMethodView>(std::move(s));
}

void Magpie::SimplexMethodScene::render()
{
    views()->render();
}

void Magpie::SimplexMethodScene::update()
{
    views()->update();
}

void Magpie::SimplexMethodScene::Next()
{

}
