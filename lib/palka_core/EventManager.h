//
// Created by NePutin on 8/21/2021.
//

#ifndef PALKA_EVENTMANAGER_H
#define PALKA_EVENTMANAGER_H

#include <set>
#include <map>
#include <functional>
#include <utility>
#include <GLFW/glfw3.h>

namespace palka
{
    enum EventType
    {
        NONE,
        KEYDOWN,
        KEYUP,
        MOUSEBDOWN,
        MOUSEBUP,
        MOUSESCROLL,
        MOUSEMOTION,
        WINDOWRESIZE,
        WINDOWMOTION,
        WINDOWCLOSE
    };

    struct EventData
    {
        struct
        {
            int x;
            int y;
        } WindowPos;
        struct
        {
            int newX;
            int newY;
        } WindowResize;
        struct
        {
            int key;
            int action;
            int scancode;
            int mode;
        } KeyPress;
        struct
        {
            int button;
            int action;
            int mods;
        } MouseButtonPress;
        struct
        {
            int offsetX;
            int offsetY;
        } MouseScroll;
        struct
        {
            int x;
            int y;
        } MouseMotion;
    };

    struct KBoardEvent
    {
        EventType type = NONE;
        int key = -1;
        //KBoardEvent() : type(), key() {}

        KBoardEvent(EventType type, int key) : type(type), key(key)
        {}

        KBoardEvent(int key) : key(key)
        {}

        bool operator==(const KBoardEvent& other) const
        { return key == other.key && type == other.type; }

        bool operator<(const KBoardEvent& o) const
        {
            return type < o.type || (type == o.type && key < o.key);
        }

        static KBoardEvent KeyPressed(int k)
        {
            return KBoardEvent{KEYDOWN, k};
        }

        static KBoardEvent KeyReleased(int k)
        {
            return KBoardEvent{KEYUP, k};
        }
    };

    struct MouseEvent
    {
        enum Mouse_Button
        {
            None = 0,
            Left = GLFW_MOUSE_BUTTON_LEFT,
            Right = GLFW_MOUSE_BUTTON_RIGHT,
            Middle = GLFW_MOUSE_BUTTON_MIDDLE
        };

        EventType type = NONE;
        Mouse_Button key = None;

        MouseEvent(EventType type, Mouse_Button b) : key(b), type(type)
        {}

        MouseEvent(EventType type) : type(type)
        {}

        MouseEvent(Mouse_Button b) : key(b)
        {}

        bool operator==(const MouseEvent& other) const
        { return key == other.key && type == other.type; }

        bool operator<(const MouseEvent& o) const
        {
            return (type < o.type || (type == o.type && key < o.key));
        }

        static MouseEvent ButtonPressed(Mouse_Button b)
        {
            return MouseEvent{MOUSEBDOWN, b};
        }

        static MouseEvent ButtonReleased(Mouse_Button b)
        {
            return MouseEvent{MOUSEBUP, b};
        }

        static MouseEvent WheelScrolled()
        {
            return MouseEvent{MOUSESCROLL, Mouse_Button::None};
        }

        static MouseEvent Motion()
        {
            return MouseEvent{MOUSEMOTION, Mouse_Button::None};
        }
    };

    class EventManager
    {
    private:
        inline static std::multimap<KBoardEvent, std::function<void(EventData&)>> KeyboardEvents{};
        inline static std::multimap<KBoardEvent, std::function<void()>> KeyboardInputs{};
        inline static std::multimap<MouseEvent, std::function<void(EventData&)>> MouseEvents{};
        inline static std::multimap<EventType, std::function<void(EventData&)>> TypeEvents{};

        inline static std::set<int> keyPressed{};
        inline static std::set<MouseEvent::Mouse_Button> mousebPress{};
    public:

        static void addEvent(EventType t, const std::function<void(EventData&)>& callback);

        static void addEvent(KBoardEvent e, const std::function<void(EventData&)>& callback);

        static void addInput(int k, const std::function<void()>& callback);

        static void addEvent(MouseEvent e, const std::function<void(EventData&)>& callback);

        static bool isKeyPressed(int key);

        static void clearInput()
        {
            keyPressed.clear();
            mousebPress.clear();
        }

        static void MouseMotionEventHolder(GLFWwindow* window, double xpos, double ypos);

        static void MouseScrollEventHolder(GLFWwindow* window, double xoffset, double yoffset);

        static void MouseButtonEventHolder(GLFWwindow* window, int button, int action, int mods);

        static void KeyBoardEventHolder(GLFWwindow* window, int key, int scancode, int action, int mods);

        static void WindowResizeEventHolder(GLFWwindow* window, int width, int height);

        static void WindowCloseEventHolder(GLFWwindow* window);

        static void WindowMotionEventHolder(GLFWwindow* window, int xpos, int ypos);

        static void bindEvents(GLFWwindow* w);

        static void updateInputs();
    };
}

#endif //PALKA_EVENTMANAGER_H
