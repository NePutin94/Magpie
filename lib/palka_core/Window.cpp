//
// Created by NePutin on 7/25/2021.
//

#include "Window.h"

void palka::Window::create(std::string_view name, std::string_view icon_path)
{
    if(!glfwInit())
        Console::addLog("Failed to initialize GLFW", Console::fatal);
    glfwSetErrorCallback(error_callback);
    glfwDefaultWindowHints();
    window = glfwCreateWindow(size.x, size.y, name.data(), NULL, NULL);
    if(!window)
        Console::addLog("Failed to open GLFW window", Console::fatal);

    glfwMakeContextCurrent(window);
    int version = gladLoadGL(glfwGetProcAddress);
    if(version == 0)
    {
        Console::addLog("Failed to initialize OpenGL context", Console::error);
    }
    glfwSwapInterval(0);
    Console::fmt_log("Glad version: {}.{}", Console::info, GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    Console::fmt_log("GL_VERSION: {}", Console::info, std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION))));
    EventManager::bindEvents(window);
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    implot_context = ImPlot::CreateContext();
    imgui_context = ImGui::CreateContext();
    ImGui::SetCurrentContext(imgui_context);
    ImPlot::SetCurrentContext(implot_context);
    ImGuiIO& io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking

    ImGui::StyleColorsDark();


    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    if(!icon_path.empty())
    {
        GLFWimage images[1];
        images[0].pixels = stbi_load(icon_path.data(), &images[0].width, &images[0].height, 0, 4);
        glfwSetWindowIcon(window, 1, images);
        stbi_image_free(images[0].pixels);
    }
    glEnable(GL_DEPTH_TEST);
}

void palka::Window::ImGuiNewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void palka::Window::ImGuiEndFrame()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void palka::Window::eventHandler()
{
    glfwPollEvents();
}

void palka::Window::EndFrame()
{
    glfwSwapBuffers(window);
}

void palka::Window::error_callback(int error, const char* description)
{
    Console::fmt_log("Error: {}", Console::error, description);
}

palka::Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
