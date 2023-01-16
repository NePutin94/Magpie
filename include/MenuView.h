//
// Created by dimka on 9/4/2022.
//

#ifndef MAGPIE_MENUVIEW_H
#define MAGPIE_MENUVIEW_H

#include "UiView.h"
#include <imgui.h>
#include <implot.h>
#include <fmt/format.h>
#include "config.h"
#include "imgui_internal.h"

namespace Magpie
{
    class MenuView : public UiView
    {
    private:
        States res;
        palka::Vec2f scale;
        void navLayout(bool isReady)
        {
            constexpr int b_padding = 40.f;
            ImVec2 b_size{80.f, 35.f};
            ImGui::Spacing();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x * .5f);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - b_size.x / 2.f);
            if(ImGui::Button("Back", b_size))
            {
                nextSatet = States::Back;
                sceneCallback();
            }
        }
    public:

        MenuView(std::string_view name, palka::Vec2f scale) : UiView(name, Config::WindowSize * scale), scale(scale)
        {
        }

        void render(palka::Window& w) override
        {
            size = {Config::WindowSize.x * scale.x, Config::WindowSize.y * scale.y};
            auto menuWindowPos = ImVec2((Config::WindowSize.x - (size.x)) / 2,
                                        (Config::WindowSize.y - (size.y)) / 2);
            ImGui::SetNextWindowPos(menuWindowPos, ImGuiCond_Always, {0, 0});
            if(ImGui::Begin("Menu"))
            {
                ImGui::SetWindowSize({size.x, size.y});
                static int item_current_idx = 0;
                constexpr auto itemWidth = 230.f;
                constexpr auto yOffsetConst = 50.f;
                constexpr auto itemHeight = 36.f;
                constexpr auto menuButtonSz = ImVec2{itemWidth, 36};
                auto yOffset = 100.f;

                ImGui::PushItemWidth(itemWidth);
                ImGui::SetCursorPosX((ImGui::GetWindowWidth() - itemWidth) / 2.f);
                ImGui::SetCursorPosY(yOffset);
                std::string_view combo_preview_value = storage->types[item_current_idx];
                if(ImGui::BeginCombo("Data type", combo_preview_value.data()))
                {
                    for(int i = 0; i < storage->types.size(); i++)
                    {
                        const bool is_selected = (item_current_idx == i);
                        if(ImGui::Selectable(storage->types[i].data(), is_selected))
                        {
                            storage->type = (DataStorage::DataType) i;
                            item_current_idx = i;
                        }
                        if(is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                ImGui::PopItemWidth();

                ImGui::Spacing();
                ImGui::SetCursorPosX((ImGui::GetWindowWidth() - itemWidth) / 2.f);
                if(ImGui::Button("Graphical method", menuButtonSz))
                {
                    nextSatet = States::SolverGraphics;
                    sceneCallback();
                }
                ImGui::Spacing();
                ImGui::SetCursorPosX((ImGui::GetWindowWidth() - itemWidth) / 2.f);
                if(ImGui::Button("Graphics 3D", menuButtonSz))
                {
                    nextSatet = States::SolverGraphics3D;
                    sceneCallback();
                }
                ImGui::Spacing();
                ImGui::SetCursorPosX((ImGui::GetWindowWidth() - itemWidth) / 2.f);
                if(ImGui::Button("Simplex method", menuButtonSz))
                {
                    nextSatet = States::SolverSimplex;
                    sceneCallback();
                }
                ImGui::Spacing();
                ImGui::SetCursorPosX((ImGui::GetWindowWidth() - itemWidth) / 2.f);
                if(ImGui::Button("Artificial Basis", menuButtonSz))
                {
                    nextSatet = States::SolverSimplexArtificialBasis;
                    sceneCallback();
                }
                ImGui::Spacing();
                ImGui::SetCursorPosX((ImGui::GetWindowWidth() - itemWidth) / 2.f);
                if(ImGui::Button("Simplex method (one pass)", menuButtonSz))
                {
                    nextSatet = States::SolverSimplexOnePass;
                    sceneCallback();
                }
                navLayout(true);
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
