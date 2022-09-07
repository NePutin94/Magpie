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
#include <Tracy.hpp>
#include "StartScene.h"
#include "SceneManager.h"


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
        SceneManager manager;
    public:
        explicit Application(Vec2i size) : w(size), isRuning(false), view(RectF(0, 0, size.x, size.y))
        {
            init();
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

        void init();

        void render()
        {
            w.clear();
            ImPlot::ShowDemoWindow();
            manager.getScene()->render();
            Console::Draw("Console", &console_open);
            w.ImGuiEndFrame();
            w.EndFrame();
        }

        void update()
        {
            // debug(w.getCamera(), "camera");
            // debug(w.getViewport(), "view");
            manager.getScene()->update();
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
