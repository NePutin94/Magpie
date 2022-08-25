//
// Created by NePutin on 1/22/2022.
//

#ifndef BILLET_APPLICATION_H
#define BILLET_APPLICATION_H

#include <Window.h>
#include <VertexBuffer.h>
#include <ReflectionDebug.h>
#include "Test.h"
#include "DockWrapper.h"
#include "MainScene.h"
#include <imgui_internal.h>

using namespace palka;
namespace Magpie
{
    class Application
    {
    private:
        bool console_open = false;
        Viewport view;
        Window w;

        bool isRuning = true;

        double oldTimeSinceStart = 0;
        double timeSinceStart;
        float t = 0;
        float delta;
        MainScene scene;

    public:
        explicit Application(Vec2i size) : w(size), isRuning(false), view(RectF(0, 0, size.x, size.y))
        {
            init();
            scene.init();

        }

        void mainApp()
        {
        }

        static void glfw_error_callback(int error, const char* description)
        {
            fprintf(stderr, "Glfw Error %d: %s\n", error, description);
        }

        void run()
        {
            while(isRuning)
            {
                handleEvents();
                w.ImGUiNewFrame();
                update();
                render();
            }
        }

        void init()
        {
            w.create();
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

        void render()
        {
            w.clear();
            mainApp();
            scene.render();
            Console::Draw("Console", &console_open);
            w.ImGuiEndFrame();
            w.EndFrame();
        }

        void update()
        {
            // debug(w.getCamera(), "camera");
            // debug(w.getViewport(), "view");
            scene.update();
            timeSinceStart = glfwGetTime();
            delta = timeSinceStart - oldTimeSinceStart;
            oldTimeSinceStart = timeSinceStart;
            EventManager::updateInputs();
        }

        void handleEvents()
        {
            w.eventHandler();
        }
    };
}

#endif //BILLET_APPLICATION_H
