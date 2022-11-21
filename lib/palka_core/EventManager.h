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
    enum class _EventType
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

    class EventType
    {
    private:
        static inline unsigned int _id = 0;
        unsigned int id;
        _EventType type;
    public:
        EventType(_EventType type) : type(type)
        { id = EventType::_id++; }

        operator _EventType() const
        {
            return type;
        }

        unsigned int getId() const
        {
            return id;
        }
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
        static inline unsigned int _id = 0;
        unsigned int id;
        EventType type = _EventType::NONE;
        int key = -1;
        //KBoardEvent() : type(), key() {}

        KBoardEvent(EventType type, int key) : type(type), key(key)
        { id = KBoardEvent::_id++; }

        KBoardEvent(int key) : key(key)
        { id = KBoardEvent::_id++; }

        unsigned int getId() const
        {
            return id;
        }

        bool operator==(const KBoardEvent& other) const
        { return key == other.key && type == other.type; }

        bool operator<(const KBoardEvent& o) const
        {
            return type < o.type || (type == o.type && key < o.key);
        }

        static KBoardEvent KeyPressed(int k)
        {
            return KBoardEvent{_EventType::KEYDOWN, k};
        }

        static KBoardEvent KeyReleased(int k)
        {
            return KBoardEvent{_EventType::KEYUP, k};
        }
    };

    struct MouseEvent
    {
        static inline unsigned int _id = 0;
        unsigned int id;

        enum Mouse_Button
        {
            None = 0,
            Left = GLFW_MOUSE_BUTTON_LEFT,
            Right = GLFW_MOUSE_BUTTON_RIGHT,
            Middle = GLFW_MOUSE_BUTTON_MIDDLE
        };

        EventType type = _EventType::NONE;
        Mouse_Button key = None;

        MouseEvent(EventType type, Mouse_Button b) : key(b), type(type)
        { id = MouseEvent::_id++; }

        MouseEvent(EventType type) : type(type)
        { id = MouseEvent::_id++; }

        MouseEvent(Mouse_Button b) : key(b)
        { id = MouseEvent::_id++; }

        unsigned int getId() const
        {
            return id;
        }

        bool operator==(const MouseEvent& other) const
        { return key == other.key && type == other.type; }

        bool operator<(const MouseEvent& o) const
        {
            return (type < o.type || (type == o.type && key < o.key));
        }

        static MouseEvent ButtonPressed(Mouse_Button b)
        {
            return MouseEvent{_EventType::MOUSEBDOWN, b};
        }

        static MouseEvent ButtonReleased(Mouse_Button b)
        {
            return MouseEvent{_EventType::MOUSEBUP, b};
        }

        static MouseEvent WheelScrolled()
        {
            return MouseEvent{_EventType::MOUSESCROLL, Mouse_Button::None};
        }

        static MouseEvent Motion()
        {
            return MouseEvent{_EventType::MOUSEMOTION, Mouse_Button::None};
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

        static void deleteKBoasrdInput(unsigned int id)
        {
            std::erase_if(KeyboardInputs, [id](const std::pair<KBoardEvent, std::function<void()>>& item)
            {
                auto const& [key, value] = item;
                return key.getId() == id;
            });
        }

        static void deleteEvent(unsigned int id)
        {
            std::erase_if(TypeEvents, [id](const std::pair<EventType, std::function<void(EventData&)>>& item)
            {
                auto const& [key, value] = item;
                return key.getId() == id;
            });
        }

        static void deleteKBoardEvent(unsigned int id)
        {
            std::erase_if(KeyboardEvents, [id](const std::pair<KBoardEvent, std::function<void(EventData&)>>& item)
            {
                auto const& [key, value] = item;
                return key.getId() == id;
            });
        }

        static void deleteMouseEvent(unsigned int id)
        {
            std::erase_if(MouseEvents, [id](const std::pair<MouseEvent, std::function<void(EventData&)>>& item)
            {
                auto const& [key, value] = item;
                return key.getId() == id;
            });
        }

        static unsigned int addEvent(EventType t, const std::function<void(EventData&)>& callback);

        static  unsigned int addEvent(KBoardEvent e, const std::function<void(EventData&)>& callback);

        static  unsigned int addInput(int k, const std::function<void()>& callback);

        static  unsigned int addEvent(MouseEvent e, const std::function<void(EventData&)>& callback);

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
