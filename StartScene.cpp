//
// Created by dimka on 9/4/2022.
//

#include "StartScene.h"
#include "MenuView.h"

void Magpie::StartScene::init()
{
    addView<MenuView>(MenuView("MagicInput", Magpie::Config::WindowSize * palka::Vec2f {0.8, 0.8}));
}

int Magpie::StartScene::getResult()
{
    return (int)getView<MenuView>()->getResult();
}
