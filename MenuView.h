//
// Created by dimka on 9/4/2022.
//

#ifndef MAGPIE_MENUVIEW_H
#define MAGPIE_MENUVIEW_H

#include "UiView.h"
#include <imgui_internal.h>
#include <implot.h>
#include <fmt/core.h>
#include "SceneManager.h"

namespace Magpie
{
    class MenuView : public UiView
    {
    private:
    SceneManager::States res;
    public:
        void render() override
        {
            if(ImGui::Begin("Menu"))
            {
                if(ImGui::Button("Input"))
                {
                    res = SceneManager::States::Solver1;
                    sceneCallback();
                }
                if(ImGui::Button("Input2"))
                {
                    res = SceneManager::States::Solver2;
                    sceneCallback();
                }
                ImGui::End();
            }
        }

        void update() override
        {}

        void setEvents() override
        {}

        auto getResult()
        {
            return res;
        }
    };
}
#endif //MAGPIE_MENUVIEW_H
