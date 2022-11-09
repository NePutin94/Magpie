#ifndef MAGPIE_GRAPHICSMETHODSCENE_H
#define MAGPIE_GRAPHICSMETHODSCENE_H

#include "UiScene.h"
#include "GraphicalMethodView.h"
#include "MagicInput.h"
#include "config.h"

namespace Magpie
{
    class GraphicsMethodScene : public UiScene
    {
    public:
        enum class State
        {
            Input = 0,
            Solve
        };
    private:
        State state;
    public:
        GraphicsMethodScene()
        {
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
                    addView<MagicInput>(MagicInput("MagicInput", Magpie::Config::WindowSize * palka::Vec2f {0.9, 0.9}));
                }
                    break;
                case State::Solve:
                {
                    auto res = getView<MagicInput>()->getResult();
                    addView<GraphicalMethodView>({{0, 0}, Magpie::Config::WindowSize * palka::Vec2f{0.9, 0.9}, res});
                }
                    break;
                default:
                    state = State::Input;
            }
        }

        void init() override
        {
            state = State::Input;
            addView<MagicInput>(MagicInput("MagicInput", Magpie::Config::WindowSize * palka::Vec2f {0.9, 0.9}));
            getView<MagicInput>()->setEvents();
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

#endif //MAGPIE_GRAPHICSMETHODSCENE_H
