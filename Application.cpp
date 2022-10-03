#include "Application.h"
#include "IcoHolder.h"

void Magpie::Application::init()
{
    w.create("Magpie", "magpie.png");

    icoHolder.init();

    isRuning = true;
    palka::EventManager::addEvent(palka::KBoardEvent::KeyPressed(GLFW_KEY_GRAVE_ACCENT), [this](palka::EventData e)
    {
        console_open = !console_open;
    });
    palka::EventManager::addEvent(palka::EventType::WINDOWCLOSE, [this](palka::EventData e)
    {
        isRuning = false;
    });
}
