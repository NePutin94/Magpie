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
        static std::multimap<KBoardEvent, std::function<void(EventData&)>> KeyboardEvents;
        static std::multimap<KBoardEvent, std::function<void()>> KeyboardInputs;
        static std::multimap<MouseEvent, std::function<void(EventData&)>> MouseEvents;
        static std::multimap<EventType, std::function<void(EventData&)>> TypeEvents;

        static std::set<int> keyPressed;
        static std::set <MouseEvent::Mouse_Button> mousebPress;
    public:

        static void addEvent(EventType t, const std::function<void(EventData&)>& callback)
        {
            TypeEvents.emplace(t, callback);
        }

        static void addEvent(KBoardEvent e, const std::function<void(EventData&)>& callback)
        {
            KeyboardEvents.emplace(e, callback);
        }

        static void addInput(int k, const std::function<void()>& callback)
        {
            KeyboardInputs.emplace(KBoardEvent::KeyPressed(k), callback);
        }

        static void addEvent(MouseEvent e, const std::function<void(EventData&)>& callback)
        {
            MouseEvents.emplace(e, callback);
        }

        static bool isKeyPressed(int key)
        {
            return keyPressed.count(key) == 1;
        }

        static void clearInput()
        {
            keyPressed.clear();
            mousebPress.clear();
        }

        static void MouseMotionEventHolder(GLFWwindow* window, double xpos, double ypos)
        {
            EventData data;
            data.MouseMotion.x = xpos;
            data.MouseMotion.y = ypos;
            auto range = MouseEvents.equal_range(MouseEvent::Motion());
            for (auto it = range.first; it != range.second; ++it)
                it->second(data);
        }

        static void MouseScrollEventHolder(GLFWwindow* window, double xoffset, double yoffset)
        {
            EventData data;
            data.MouseScroll.offsetX = xoffset;
            data.MouseScroll.offsetY = yoffset;
            auto range = MouseEvents.equal_range(MouseEvent::WheelScrolled());
            for (auto it = range.first; it != range.second; ++it)
                it->second(data);
        }

        static void MouseButtonEventHolder(GLFWwindow* window, int button, int action, int mods)
        {
            EventData data;
            data.MouseButtonPress.action = action;
            data.MouseButtonPress.button = button;
            data.MouseButtonPress.mods = mods;
            if (action == GLFW_PRESS)
            {
                mousebPress.emplace((MouseEvent::Mouse_Button) button);
                auto range = MouseEvents.equal_range(MouseEvent{MOUSEBDOWN, (MouseEvent::Mouse_Button) button});
                for (auto it = range.first; it != range.second; ++it)
                    it->second(data);
            } else if (action == GLFW_RELEASE)
            {
                auto range = MouseEvents.equal_range(MouseEvent{MOUSEBUP, (MouseEvent::Mouse_Button) button});
                for (auto it = range.first; it != range.second; ++it)
                    it->second(data);
            }
        }

        static void KeyBoardEventHolder(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            EventData data;
            data.KeyPress.action = action;
            data.KeyPress.key = key;
            data.KeyPress.mode = mods;
            data.KeyPress.scancode = scancode;
            if (action == GLFW_PRESS)
            {
                keyPressed.emplace(key);
                auto range = KeyboardEvents.equal_range(KBoardEvent{EventType::KEYDOWN, key});
                for (auto it = range.first; it != range.second; ++it)
                    it->second(data);
            } else if (action == GLFW_RELEASE)
            {
                keyPressed.erase(key);
                auto range = KeyboardEvents.equal_range(KBoardEvent{EventType::KEYUP, key});
                for (auto it = range.first; it != range.second; ++it)
                    it->second(data);
            }
        }

        static void WindowResizeEventHolder(GLFWwindow* window, int width, int height)
        {
            EventData data;
            data.WindowResize.newX = width;
            data.WindowResize.newY = height;
            auto range = TypeEvents.equal_range(WINDOWRESIZE);
            for (auto it = range.first; it != range.second; ++it)
                it->second(data);
        }

        static void WindowCloseEventHolder(GLFWwindow* window)
        {
            EventData data;
            auto range = TypeEvents.equal_range(WINDOWCLOSE);
            for (auto it = range.first; it != range.second; ++it)
                it->second(data);
        }

        static void WindowMotionEventHolder(GLFWwindow* window, int xpos, int ypos)
        {
            EventData data;
            data.WindowPos.x = xpos;
            data.WindowPos.y = ypos;
            auto range = TypeEvents.equal_range(WINDOWMOTION);
            for (auto it = range.first; it != range.second; ++it)
                it->second(data);
        }

        static void bindEvents(GLFWwindow* w)
        {
            glfwSetMouseButtonCallback(w, &EventManager::MouseButtonEventHolder);
            glfwSetCursorPosCallback(w, &EventManager::MouseMotionEventHolder);
            glfwSetKeyCallback(w, &EventManager::KeyBoardEventHolder);
            glfwSetScrollCallback(w, &EventManager::MouseScrollEventHolder);
            glfwSetWindowCloseCallback(w, &EventManager::WindowCloseEventHolder);
        }

        static void updateInputs()
        {
            for(auto& i : KeyboardInputs)
                if(keyPressed.contains(i.first.key))
                    i.second();
        }
    };
}

#endif //PALKA_EVENTMANAGER_H
