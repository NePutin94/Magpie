//
// Created by dimka on 9/4/2022.
//

#ifndef MAGPIE_SCENEMANAGER_H
#define MAGPIE_SCENEMANAGER_H

#include "UiScene.h"
#include "StartScene.h"
#include "MainScene.h"
#include "GraphicsMethodScene.h"
#include "SimpleMethodScene.h"

namespace Magpie
{
    class SceneManager
    {
    public:
        enum class States
        {
            Menu,
            Solver1,
            Solver2,
            SimpleMet
        };
    private:
        std::unique_ptr<UiScene> curr;
        bool close = false;
        States state;
    private:
        void Next()
        {
            state = (States)getScene<StartScene>()->getResult();
            switch(state)
            {
                case States::Menu:
                    AddScene<StartScene>();
                    break;
                case States::Solver1:
                    AddScene<MainScene>();
                    break;
                case States::Solver2:
                    AddScene<GraphicsMethodScene>();
                    break;
                case States::SimpleMet:
                    AddScene<SimpleMethodScene>();
                    break;
            }
        }

        template<class T>
        void AddScene()
        {
            curr = std::move(std::make_unique<T>());
            curr->init();
        }

    public:
        SceneManager()
        {
            state = States::Menu;
            AddScene<StartScene>();
        }

        auto scene()
        {
            return curr.get();
        }

        template<class T>
        T* getScene()
        {
            return static_cast<T*>(curr.get());
        }

        UiScene* getScene()
        {
            if(curr->IsClose())
            {
                Next();
            }
            return curr.get();
        }

    };
}

#endif //MAGPIE_SCENEMANAGER_H
