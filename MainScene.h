//
// Created by dimka on 3/10/2022.
//

#ifndef BILLET_MAINSCENE_H
#define BILLET_MAINSCENE_H

#include "UiScene.h"
#include "Test.h"

namespace Billet
{
    class MainScene : protected UiScene
    {
    public:

        void init() override
        {
            addView<Test>("solve", {{0,   0},
                                    {520, 120}});
        }

        void render() override
        {
            for(auto& [_, v]: views())
            {
                v->render();
            }
        }

        void update() override
        {
            for(auto& [_, v]: views())
            {
                v->update();
            }
        }

    };
}

#endif //BILLET_MAINSCENE_H
