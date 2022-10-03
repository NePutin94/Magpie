#ifndef MAGPIE_SIMPLEMETHODSCENE_H
#define MAGPIE_SIMPLEMETHODSCENE_H

#include "UiScene.h"

namespace Magpie
{
    class SimpleMethodScene: public UiScene
    {
    public:
        ~SimpleMethodScene() override = default;

        void init() override;

        void render() override;

        void update() override;

    protected:
        void Next() override;
    };
}
#endif //MAGPIE_SIMPLEMETHODSCENE_H
