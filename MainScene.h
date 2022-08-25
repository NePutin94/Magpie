#ifndef BILLET_MAINSCENE_H
#define BILLET_MAINSCENE_H

#include "UiScene.h"
#include "Test.h"
#include "InputView.h"
#include "GraphicalMethodView.h"

namespace Magpie
{
    class MainScene : protected UiScene
    {
    private:
        enum class State
        {
            Input = 0,
            InputRest,
            Solve
        };
    private:
        State state;
    public:

        void Next() override
        {
            state = (State) ((int) state + 1);
            switch(state)
            {
                case State::Input:
                {
                    addView<InputView>({{0,   0},
                                        {520, 120}});
                }
                    break;
                case State::InputRest:
                {
                    auto res = getView<InputView>()->getResul();
                    addView<Test>({{0, 0}, {520, 120}, res.first, res.second});
                }
                    break;
                case State::Solve:
                {
                    auto res = getView<Test>()->getResult();
                    addView<GraphicalMethodView>({{0, 0}, {520, 120}, res});
                }
                    break;
                default:
                    state = State::Input;
            }
        }

        void init() override
        {
            state = State::Input;
            addView<InputView>({{0,   0},
                                {520, 120}});
        }

        void render() override
        {
            views()->render();
        }

        void update() override
        {
            views()->update();
        }
    };
}

#endif //BILLET_MAINSCENE_H
