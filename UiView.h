//
// Created by dimka on 1/29/2022.
//

#ifndef BILLET_UIVIEW_H
#define BILLET_UIVIEW_H

#include <imgui.h>
#include <string>

#define IMGUI_DEFINE_MATH_OPERATORS

#include <Vec2.h>

namespace Billet
{
    class UiView
    {
    protected:
        ImGuiWindowFlags win_flag;
        std::string name;
        palka::Vec2f size;
        palka::Vec2f pos;
        bool open;
    public:
        UiView() = default;

        virtual ~UiView() = default;

        UiView(std::string_view name, bool open = true, ImGuiWindowFlags w_flag = ImGuiWindowFlags_None)
                : name(name), win_flag(w_flag), open(open)
        {}

        UiView(std::string_view name, palka::Vec2f pos, palka::Vec2f size, bool open = true, ImGuiWindowFlags w_flag = ImGuiWindowFlags_None)
                : name(name), win_flag(w_flag), open(open), pos(pos), size(size)
        {}

        std::string_view getName() const
        {
            return name;
        }

        palka::Vec2f getSize() const
        {
            return size;
        }

        virtual void render() = 0;

        virtual void update() = 0;

        virtual void setEvents() = 0;
    };
}

#endif //BILLET_UIVIEW_H
