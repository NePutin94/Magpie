//
// Created by NePutin on 7/25/2021.
//

#ifndef PALKA_WINDOW_H
#define PALKA_WINDOW_H

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <implot.h>

#include "Vec2.h"
#include "Color.h"
#include "Viewport.h"
#include "ConsoleLog.h"
#include "EventManager.h"
#include "RenderContext.h"
#include "VertexData.h"
#include "Renderer.h"
#include "stb_image.h"

namespace palka
{
    class Window : public Renderer
    {
    private:
        Vec2i size;
        GLFWwindow* window;
        EventManager eManager;
    public:
        ImGuiContext* imgui_context;
        ImPlotContext* implot_context;
        static void error_callback(int error, const char* description);

        Window(const Vec2i& size) : Renderer(size), size(size)
        {}

        ~Window();

        void create(std::string_view name, std::string_view icon_path = "");

        GLFWwindow* getWindow()
        {
            return window;
        }

        auto getSize()
        {
            return size;
        }

        void ImGuiNewFrame();

        void ImGuiEndFrame();

        void EndFrame();

        EventManager& getEManager()
        {
            return eManager;
        }

        void eventHandler();

    };
}
#endif //PALKA_WINDOW_H
