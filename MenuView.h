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

        MenuView(std::string_view name, palka::Vec2f size) : UiView(name, size)
        {

        }

        void render() override
        {
            ImGui::SetNextWindowPos(ImVec2((Config::WindowSize.x - (size.x)) / 2,
                                           (Config::WindowSize.y - (size.y)) / 2), ImGuiCond_Always, {0, 0});
            if(ImGui::Begin("Menu"))
            {
                ImGui::SetWindowSize({size.x, size.y});
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
                if(ImGui::Button("Simplexmet"))
                {
                    res = SceneManager::States::SimpleMet;
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
