//
// Created by NePutin on 6/30/2021.
//

#ifndef PALKA_DEBUGDRAW_H
#define PALKA_DEBUGDRAW_H

#include <imgui.h>
#include <rttr/type>
#include <vector>

#include "Vec2.h"
#include "Rect.h"
#include "Color.h"
#include "Sprite.h"
#include "Rectangle.h"
#include "Window.h"

namespace palka
{
    class DebugDraw
    {
    RTTR_ENABLE()
    public:

        static void DrawBox()
        {

        }

        static void DrawSpriteDebug(Sprite& sp, Window& w)
        {
            w.draw(sp);
            auto r2 = sp.getLocalRect();
            auto r = sp.getQuad(RectF(r2.left, r2.top, r2.w, r2.h));
            Rectangle rect({0,0, (float)r2.w, (float)r2.h});
            rect.copy(sp);
            //rect.setPosition(sp.getPosition());
            w.draw(rect);
        }

        enum Pos
        {
            LeftSideTop,
            LeftSideBottom,
            RightSideTop,
            RightSideBottom
        };

        static void SimpleText(Vec2i position, bool* open, std::string_view name, std::string_view text, Pos pos)
        {
            ImVec2 size = {128, 48};
            ImVec2 window_pos{};
            switch (pos)
            {
                case LeftSideTop:
                    window_pos = ImVec2(position.x - size.x, position.y - size.y);
                    break;
                case LeftSideBottom:
                    window_pos = ImVec2(position.x - size.x, position.y);
                    break;
                case RightSideTop:
                    window_pos = ImVec2(position.x, position.y - size.y);
                    break;
                case RightSideBottom:
                    window_pos = ImVec2(position.x, position.y);
                    break;
            }
            ImGui::SetNextWindowSize(size);
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(0, 0));
            ImGui::SetNextWindowBgAlpha(0.6f);
            if (ImGui::Begin(name.data(), open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar |
                                                 ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                                 ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
            {
                if (!text.empty())
                    ImGui::Text("%s", text.data());
                //ImGui::Separator();
                ImGui::Text("(X,Y): (%.1d, %.1d)", position.x, position.y);
                ImGui::End();
            }

        }

        static void SimpleTextF(Vec2f position, bool* open, std::string name, std::string text, Pos pos)
        {
            ImVec2 size = {128, 48};
            ImVec2 window_pos{};
            switch (pos)
            {
                case LeftSideTop:
                    window_pos = ImVec2(position.x - size.x, position.y - size.y);
                    break;
                case LeftSideBottom:
                    window_pos = ImVec2(position.x - size.x, position.y);
                    break;
                case RightSideTop:
                    window_pos = ImVec2(position.x, position.y - size.y);
                    break;
                case RightSideBottom:
                    window_pos = ImVec2(position.x, position.y);
                    break;
            }
            ImGui::SetNextWindowSize(size);
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, ImVec2(0, 0));
            ImGui::SetNextWindowBgAlpha(0.6f);
            if (ImGui::Begin(name.c_str(), open, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar |
                                                 ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                                 ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
            {
                if (!text.empty())
                    ImGui::Text("%s", text.c_str());
                //ImGui::Separator();
                ImGui::Text("(X,Y): (%.1d, %.1d)", position.x, position.y);
                ImGui::End();
            }
        }

        static Color point_color;
        static Color line_color;
    };
}

#endif //PALKA_DEBUGDRAW_H
