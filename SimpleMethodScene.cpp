#include "SimpleMethodScene.h"
#include "SimplexMethodView.h"

void Magpie::SimpleMethodScene::init()
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

    addView<SimplexMethodView>(SimplexMethodView(in));
}

void Magpie::SimpleMethodScene::render()
{
    views()->render();
}

void Magpie::SimpleMethodScene::update()
{
    views()->update();
}

void Magpie::SimpleMethodScene::Next()
{

}
