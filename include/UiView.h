#ifndef BILLET_UIVIEW_H
#define BILLET_UIVIEW_H

#include <imgui.h>
#include <string>
#include <Vec2.h>

namespace Magpie
{
    class UiView
    {
    protected:
        ImGuiWindowFlags win_flag;
        std::string name;
        palka::Vec2f size;
        palka::Vec2f pos;
        bool open;

        std::function<void()> sceneCallback; //to link UiScene and UIView, it is expected to be called when you need to switch UIView in the UiScene
    public:
        UiView() = default;

        virtual ~UiView() = default;

        UiView(std::string_view name, bool open = true, ImGuiWindowFlags w_flag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)
                : name(name), win_flag(w_flag), open(open)
        {}

        UiView(std::string_view name, palka::Vec2f size, bool open = true, ImGuiWindowFlags w_flag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)
                : name(name), win_flag(w_flag), open(open), size(size)
        {}

        UiView(std::string_view name, palka::Vec2f pos, palka::Vec2f size, bool open = true,
               ImGuiWindowFlags w_flag = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove)
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

        void setCallback(std::function<void()> call)
        {
            sceneCallback = call;
        }

        virtual void render() = 0;

        virtual void update() = 0;

        virtual void setEvents() = 0;
    };
}

#endif //BILLET_UIVIEW_H
