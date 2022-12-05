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
//                    addView<InputView>({{0, 0},
//                                        Magpie::Config::WindowSize * palka::Vec2f{0.9, 0.9}});
                }
                    break;
                case State::InputRest:
                {
                 //   auto res = getView<InputView>()->getResult();
                 //   addView<Test>({{0, 0}, Magpie::Config::WindowSize * palka::Vec2f{0.9, 0.9}, res.first, res.second});
                }
                    break;
                case State::Solve:
                {
                    auto res = getView<Test>()->getResult();
//                    GraphicalMethodView s{{0, 0}, Magpie::Config::WindowSize * palka::Vec2f{0.9, 0.9}, res};
              //      addView<GraphicalMethodView>(std::move(s));
                }
                    break;
                default:
                    state = State::Input;
            }
        }

        void init() override
        {
            state = State::Input;
//            addView<InputView>({{0, 0},
//                                Magpie::Config::WindowSize * palka::Vec2f{0.9, 0.9}});
        }

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

#endif //BILLET_MAINSCENE_H
