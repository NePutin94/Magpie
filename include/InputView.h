#ifndef MAGPIE_INPUTVIEW_H
#define MAGPIE_INPUTVIEW_H

#include "UiView.h"
#include "config.h"
#include <implot.h>
#include <tracy/Tracy.hpp>

namespace Magpie
{
    class InputView : public UiView
    {
    private:
        int cols;
        int rows;
    public:
        InputView(std::string_view name, palka::Vec2f size) : UiView(name, size), rows(0), cols(0)
        {}

        void render(palka::Window& w) override
        {
            ImGui::SetNextWindowPos(ImVec2((Config::WindowSize.x - (size.x)) / 2,
                                           (Config::WindowSize.y - (size.y)) / 2), ImGuiCond_Always, {0, 0});
            if(ImGui::Begin(name.c_str()))
            {
                ImGui::SetWindowSize({size.x, size.y});
                ImGui::PushItemWidth(100);

                ImGui::Spacing();
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Enter the initial data.").x) / 2);
                ImGui::SetCursorPosY((ImGui::GetWindowSize().y - (ImGui::GetWindowSize().y / 2)) / 2);
                ImGui::Text("Enter the initial data.");
                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Number of variables (0<n<=5)").x - 101) / 2);
                ImGui::DragInt("Number of variables (0<n<=5)", &cols, 1, 0, 5);
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Number of variables (0<n<=5)").x - 101) / 2);
                ImGui::DragInt("Number of restrictions (0<n<=5)", &rows, 1, 0, 5);

                ImGui::Spacing();
                ImGui::Spacing();
                auto b_size = ImGui::CalcTextSize("Entering restriction (graphics 2d)").x + 20.f;
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - b_size * 2 - 15.f) / 2);
                if(ImGui::Button("Entering restriction (matrix)", {b_size, 0}))
                {
                    nextSatet = States::InputRestriction;
                    storage->data.alloc_matrix(rows, cols);
                    sceneCallback();
                }
                ImGui::SameLine();
                if(ImGui::Button("Entering restriction (graphics 2d)", {b_size, 0}))
                {
                    nextSatet = States::InputRestrictionGraph;
                    storage->data.alloc_matrix(rows, cols);
                    sceneCallback();
                }
                ImGui::PopItemWidth();
                ImGui::End();
            }
        }

        std::pair<int, int> getResult()
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
