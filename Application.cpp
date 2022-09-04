//
// Created by dimka on 1/22/2022.
//

#include "Application.h"

void Magpie::Application::init()
{
    w.create("Magpie", "magpie.png");

    isRuning = true;
    EventManager::addEvent(KBoardEvent::KeyPressed(GLFW_KEY_GRAVE_ACCENT), [this](EventData e)
    {
        console_open = !console_open;
    });
    EventManager::addEvent(EventType::WINDOWCLOSE, [this](EventData e)
    {
        isRuning = false;
    });
}
