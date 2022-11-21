//
// Created by dimka on 9/4/2022.
//

#ifndef MAGPIE_STARTSCENE_H
#define MAGPIE_STARTSCENE_H

#include "UiScene.h"


namespace Magpie
{
    class StartScene : public UiScene
    {
    private:

    public:

        void Next() override
        {
            close = true;
        }

        int getResult();

        void init() override;

        void render(palka::Window& w) override
        {
            views()->render(w);
        }

        void update() override
        {
            views()->update();
        }
    };
}

#endif //MAGPIE_STARTSCENE_H
