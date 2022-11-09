#include "SimplexMethodScene.h"
#include "SimplexMethodView.h"
#include "DataSets.h"
#include "config.h"

void Magpie::SimplexMethodScene::init()
{
    SimplexMethodView s(Config::WindowSize * palka::Vec2f{0.9, 0.9}, set7(), {2,3});
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
