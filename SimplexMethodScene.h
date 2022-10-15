#ifndef MAGPIE_SIMPLEXMETHODSCENE_H
#define MAGPIE_SIMPLEXMETHODSCENE_H

#include "UiScene.h"

namespace Magpie
{
    class SimplexMethodScene: public UiScene
    {
    public:
        ~SimplexMethodScene() override = default;

        void init() override;

        void render() override;

        void update() override;

    protected:
        void Next() override;
    };
}
#endif //MAGPIE_SIMPLEXMETHODSCENE_H
