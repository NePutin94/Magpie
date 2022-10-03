#ifndef BILLET_MAINSCENE_H
#define BILLET_MAINSCENE_H

#include "UiScene.h"
#include "Test.h"
#include "InputView.h"
#include "GraphicalMethodView.h"

namespace Magpie
{
    class MainScene : public UiScene
    {
    public:
        enum class State
        {
            Input = 0,
            InputRest,
            Solve
        };
    private:
        State state;
    public:
        MainScene()
        {
        }

        MainScene(State st)
        {
            state = st;
        }

        void Next() override
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            state = (State) ((int) state + 1);
            switch(state)
            {
                case State::Input:
                {
                    addView<InputView>({{0, 0},
                                        Magpie::Config::WindowSize * palka::Vec2f{0.8, 0.8}});
                }
                    break;
                case State::InputRest:
                {
                    auto res = getView<InputView>()->getResul();
                    addView<Test>({{0, 0}, Magpie::Config::WindowSize * palka::Vec2f{0.8, 0.8}, res.first, res.second});
                }
                    break;
                case State::Solve:
                {
                    auto res = getView<Test>()->getResult();
                    addView<GraphicalMethodView>({{0, 0}, Magpie::Config::WindowSize * palka::Vec2f{0.8, 0.8}, res});
                }
                    break;
                default:
                    state = State::Input;
            }
        }

        void init() override
        {
            state = State::Input;
            addView<InputView>({{0, 0},
                                Magpie::Config::WindowSize * palka::Vec2f{0.8, 0.8}});
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
