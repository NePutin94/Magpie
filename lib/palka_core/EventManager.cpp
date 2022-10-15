//
// Created by NePutin on 8/21/2021.
//

#include "EventManager.h"

using namespace palka;

//
//std::multimap<KBoardEvent, std::function<void(EventData&)>>  EventManager::KeyboardEvents{};
//std::multimap<KBoardEvent, std::function<void()>>  EventManager::KeyboardInputs{};
//std::multimap<MouseEvent, std::function<void(EventData&)>>  EventManager::MouseEvents{};
//std::multimap<EventType, std::function<void(EventData&)>>  EventManager::TypeEvents{};
//std::set<int>  EventManager::keyPressed{};
//std::set<MouseEvent::Mouse_Button>  EventManager::mousebPress{};
void EventManager::bindEvents(GLFWwindow* w)
{
    glfwSetMouseButtonCallback(w, &EventManager::MouseButtonEventHolder);
    glfwSetCursorPosCallback(w, &EventManager::MouseMotionEventHolder);
    glfwSetKeyCallback(w, &EventManager::KeyBoardEventHolder);
    glfwSetScrollCallback(w, &EventManager::MouseScrollEventHolder);
    glfwSetWindowCloseCallback(w, &EventManager::WindowCloseEventHolder);
}

void EventManager::updateInputs()
{
    for(auto& i: KeyboardInputs)
        if(keyPressed.contains(i.first.key))
            i.second();
}

void EventManager::WindowMotionEventHolder(GLFWwindow* window, int xpos, int ypos)
{
    EventData data{};
    data.WindowPos.x = xpos;
    data.WindowPos.y = ypos;
    auto range = TypeEvents.equal_range(WINDOWMOTION);
    for(auto it = range.first; it != range.second; ++it)
        it->second(data);
}

void EventManager::WindowCloseEventHolder(GLFWwindow* window)
{
    EventData data{};
    auto range = TypeEvents.equal_range(WINDOWCLOSE);
    for(auto it = range.first; it != range.second; ++it)
        it->second(data);
}

void EventManager::WindowResizeEventHolder(GLFWwindow* window, int width, int height)
{
    EventData data{};
    data.WindowResize.newX = width;
    data.WindowResize.newY = height;
    auto range = TypeEvents.equal_range(WINDOWRESIZE);
    for(auto it = range.first; it != range.second; ++it)
        it->second(data);
}

void EventManager::KeyBoardEventHolder(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    EventData data{};
    data.KeyPress.action = action;
    data.KeyPress.key = key;
    data.KeyPress.mode = mods;
    data.KeyPress.scancode = scancode;
    if(action == GLFW_PRESS)
    {
        keyPressed.emplace(key);
        {
            auto range = KeyboardEvents.equal_range(KBoardEvent{EventType::KEYDOWN, key});
            for(auto it = range.first; it != range.second; ++it)
                it->second(data);
        }
        {
            auto range = TypeEvents.equal_range(KEYDOWN);
            for(auto it = range.first; it != range.second; ++it)
                it->second(data);
        }
    } else if(action == GLFW_RELEASE)
    {
        keyPressed.erase(key);
        auto range = KeyboardEvents.equal_range(KBoardEvent{EventType::KEYUP, key});
        for(auto it = range.first; it != range.second; ++it)
            it->second(data);
    }
}

void EventManager::MouseButtonEventHolder(GLFWwindow* window, int button, int action, int mods)
{
    EventData data{};
    data.MouseButtonPress.action = action;
    data.MouseButtonPress.button = button;
    data.MouseButtonPress.mods = mods;
    if(action == GLFW_PRESS)
    {
        mousebPress.emplace((MouseEvent::Mouse_Button) button);
        auto range = MouseEvents.equal_range(MouseEvent{MOUSEBDOWN, (MouseEvent::Mouse_Button) button});
        for(auto it = range.first; it != range.second; ++it)
            it->second(data);
    } else if(action == GLFW_RELEASE)
    {
        auto range = MouseEvents.equal_range(MouseEvent{MOUSEBUP, (MouseEvent::Mouse_Button) button});
        for(auto it = range.first; it != range.second; ++it)
            it->second(data);
    }
}

void EventManager::addEvent(EventType t, const std::function<void(EventData&)>& callback)
{
    TypeEvents.emplace(t, callback);
}

void EventManager::addEvent(KBoardEvent e, const std::function<void(EventData&)>& callback)
{
    KeyboardEvents.emplace(e, callback);
}

void EventManager::addInput(int k, const std::function<void()>& callback)
{
    KeyboardInputs.emplace(KBoardEvent::KeyPressed(k), callback);
}

void EventManager::addEvent(MouseEvent e, const std::function<void(EventData&)>& callback)
{
    MouseEvents.emplace(e, callback);
}

bool EventManager::isKeyPressed(int key)
{
    return keyPressed.count(key) == 1;
}

void EventManager::MouseMotionEventHolder(GLFWwindow* window, double xpos, double ypos)
{
    EventData data{};
    data.MouseMotion.x = xpos;
    data.MouseMotion.y = ypos;
    auto range = MouseEvents.equal_range(MouseEvent::Motion());
    for(auto it = range.first; it != range.second; ++it)
        it->second(data);
}

void EventManager::MouseScrollEventHolder(GLFWwindow* window, double xoffset, double yoffset)
{
    EventData data{};
    data.MouseScroll.offsetX = xoffset;
    data.MouseScroll.offsetY = yoffset;
    auto range = MouseEvents.equal_range(MouseEvent::WheelScrolled());
    for(auto it = range.first; it != range.second; ++it)
        it->second(data);
}

