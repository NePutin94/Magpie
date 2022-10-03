#ifndef MAGPIE_INPUTVIEW_H
#define MAGPIE_INPUTVIEW_H

#include "UiView.h"
#include "config.h"
#include <imgui_internal.h>
#include <implot.h>
#include <Tracy.hpp>
namespace Magpie
{
    class InputView : public UiView
    {
    private:
        int cols;
        int rows;
    public:
        InputView(palka::Vec2f pos, palka::Vec2f size, bool open = true, ImGuiWindowFlags w_flag = ImGuiWindowFlags_None)
                : UiView("MagicInput", pos, size, open, w_flag), rows(0), cols(0)
        {}

        void render() override
        {
            ImGui::SetNextWindowPos(ImVec2((Config::WindowSize.x - (size.x)) / 2,
                                           (Config::WindowSize.y - (size.y)) / 2), ImGuiCond_Always, {0, 0});
            if(ImGui::Begin(name.c_str()))
            {
                ImGui::SetWindowSize({size.x, size.y});
                ImGui::PushItemWidth(100);

                ImGui::Spacing();
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 200) / 2);
                ImGui::Text("Enter the initial data.");
                ImGui::Spacing();

                ImGui::SetCursorPosY((ImGui::GetWindowSize().y - (ImGui::GetWindowSize().y / 2 - 150)) / 2);
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 241) / 2);
                ImGui::DragInt("Number of variables (0<n<=5)", &cols, 1, 0, 5);
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 241) / 2);
                ImGui::DragInt("Number of restrictions (0<n<=5)", &rows, 1, 0, 5);

                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - 120) / 2);
                if(ImGui::Button("Next Step", {120, 0}))
                    sceneCallback();

                ImGui::PopItemWidth();
                ImGui::End();
            }
        }

        std::pair<int, int> getResul()
        {
            return {rows, cols};
        }

        void update() override
        {}

        void setEvents() override
        {}
    };
}

#endif //MAGPIE_INPUTVIEW_H
