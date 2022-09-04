//
// Created by dimka on 2/3/2022.
//

#ifndef BILLET_DOCKWRAPPER_H
#define BILLET_DOCKWRAPPER_H

#include <array>
#include "imgui.h"
#include "imgui_internal.h"
#include "UiView.h"

namespace Magpie
{
    class DockWrapper
    {
    public:
        enum Pin
        {
            Left = 0,
            Top,
            Right,
            Bottom
        };
    private:
        std::array<unsigned int, 4> pins;
        ImGuiID dock_id;
        bool rebuild;
        bool needUpdate = false;
        std::map<Pin, UiView*> windows;

        void make()
        {
            auto node = ImGui::DockBuilderGetNode(dock_id);
            if(node->IsSplitNode())
                ImGui::DockBuilderRemoveNodeChildNodes(node->ID);
            ImGuiID id2x, id2y;
            ImGui::DockBuilderSplitNode(node->ID, ImGuiDir_::ImGuiDir_Left, 0, &pins[Pin::Left], &id2x);
        }

    public:
        DockWrapper() : rebuild(true), dock_id(0)
        {

        }

        void addWindow(Pin pin_side, UiView* win)
        {
            windows.emplace(pin_side, win);
        }

        void refresh()
        {
            rebuild = true;
        }

        void render()
        {
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBringToFrontOnFocus
                                            | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
                                            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y));
            ImGui::SetNextWindowSize(ImVec2(800, 800));
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpaceWnd", 0, window_flags);
            ImGui::PopStyleVar(3);
            dock_id = ImGui::GetID("DockSpace");
            ImGui::DockSpace(dock_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
            if(rebuild)
            {
                //Rebuilding dock spaces if we want to restore the "original" ui
                make();
                for(auto& w: windows)
                {
                    auto sz = w.second->getSize();
                    ImGui::DockBuilderSetNodeSize(pins[w.first], {sz.x, sz.y});
                    ImGui::DockBuilderDockWindow(w.second->getName().data(), pins[w.first]);
                }
                ImGui::DockBuilderFinish(dock_id);
                rebuild = false;
            }
            ImGui::End();
        }
    };
}

#endif //BILLET_DOCKWRAPPER_H
