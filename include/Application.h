#ifndef BILLET_APPLICATION_H
#define BILLET_APPLICATION_H

#include <Window.h>
#include "InputMatrix.h"
#include "DockWrapper.h"
#include "MainScene.h"
#include <imgui.h>
#include <tracy/Tracy.hpp>
#include "StartScene.h"
#include "SceneManager.h"
#include "myImGui.h"
#include "IcoHolder.h"
#include "RenderTexture.h"
#include "Sprite.h"
#include "PlaneMesh.h"
#include "glm/gtx/quaternion.hpp"
#include <glm/gtx/vector_angle.hpp>

namespace Magpie
{
    class Application
    {
    private:
        bool console_open = false;
        palka::Viewport view;
        palka::Window w;

        bool isRuning = true;
        palka::UniformBuffer ubo;
        palka::StaticMesh m;
        palka::ShaderProgram p;
        palka::ShaderProgram def;
        palka::ShaderProgram light;
        palka::ShaderProgram material_light;
        palka::ShaderProgram material_pbr;
        palka::assimp_loader test;
        palka::gltf_loader loader;
        tinygltf::Model mm;
        palka::PlaneMesh plane;
        palka::PlaneMesh plane2;
        palka::VertArray cub;
        double oldTimeSinceStart = 0;
        double timeSinceStart;
        float t = 0;
        float delta;
        SceneManager manager;
        IcoHolder icoHolder;

        palka::RenderTexture renderTex;
        palka::Sprite renderSp;
        palka::PolygonMesh pp;
    public:
        explicit Application(palka::Vec2i size) : w(size), isRuning(false), view(palka::RectF(0, 0, size.x, size.y)), renderTex({size.x, size.y})
        {
            manager.init();

            init();
            m.init();
            ubo.create(sizeof(float[16]) * 3);

            palka::_Shader def1("Data\\Shaders\\Default.frag", palka::_Shader::FRAGMENT);
            palka::_Shader def2("Data\\Shaders\\Default.vert", palka::_Shader::VERTEX);
            def.createProgram();
            def.addShader(def1);
            def.addShader(def2);
            def.linkProgram();
            def.UBOBindingTo(def.getUBOIndex("matrixBuffer"), 0);

            palka::_Shader l3("Data\\Shaders\\material.frag", palka::_Shader::FRAGMENT);
            palka::_Shader l4("Data\\Shaders\\material.vert", palka::_Shader::VERTEX);
            material_light.createProgram();
            material_light.addShader(l3);
            material_light.addShader(l4);
            material_light.linkProgram();
            material_light.UBOBindingTo(material_light.getUBOIndex("matrixBuffer"), 0);

            ubo.bindToPoint(0);
            lx.init();
            ly.init();
            lz.init();
            renderTex.create();
            renderTex.getViewport().setCenter({1920.f / 2.f, 1080.f / 2.f});
            pp.init({palka::Vec3f{0, 0, 0}, palka::Vec3f{1, 0, 0}, palka::Vec3f{0, 1, 0},
                     palka::Vec3f{0, 0, 1}, palka::Vec3f{0, 0, 0}});
            //  renderTex.getCamera() = 0.005f;
        }

        palka::Renderer::Line lx{{0, 0, 0},
                                 {10, 0, 0},
                                 palka::Color(255, 0, 0)};
        palka::Renderer::Line ly{{0, 0, 0},
                                 {0, 10, 0},
                                 palka::Color(0, 255, 0)};
        palka::Renderer::Line lz{{0, 0, 0},
                                 {0, 0, 10},
                                 palka::Color(0, 0, 255)};

        void setTheme();

        static void glfw_error_callback(int error, const char* description)
        {
            fprintf(stderr, "Glfw Error %d: %s\n", error, description);
        }

        void run()
        {
            while(isRuning)
            {
                handleEvents();
                w.ImGuiNewFrame();
                update();
                render();
            }
        }

        void init();

        void render()
        {

            w.clear();
//            static bool move = false;
//            static palka::Vec2f delta2d;
//            static palka::Vec3f ambient = palka::Vec3f{1.0f, 0.5f, 0.31f};
//            static palka::Vec3f diffuse = palka::Vec3f{1.0f, 0.5f, 0.31f};
//            static palka::Vec3f specular = palka::Vec3f{0.5f, 0.5f, 0.5f};
//            static palka::Vec3f l_ambient = palka::Vec3f{0.2f, 0.2f, 0.2f};
//            static palka::Vec3f l_diffuse = palka::Vec3f{0.5f, 0.5f, 0.5f};
//            static palka::Vec3f l_specular = palka::Vec3f{1.0f, 1.0f, 1.0f};
//            static bool rotatingCamera = false;
//            static bool movingCamera = false;
//            static double prevMousePosX = 0.0;
//            static double prevMousePosY = 0.0;
//            static double curMousePosX = 0.0;
//            static double curMousePosY = 0.0;
//            const auto leftMouseButtonState = glfwGetMouseButton(w.getWindow(), GLFW_MOUSE_BUTTON_LEFT);
//            if(leftMouseButtonState == GLFW_PRESS)
//            {
//                if(!rotatingCamera && !movingCamera)
//                {
//                    rotatingCamera = true;
//                    glfwGetCursorPos(w.getWindow(), &prevMousePosX, &prevMousePosY);
//                }
//            } else if(leftMouseButtonState == GLFW_RELEASE)
//            {
//                rotatingCamera = false;
//            }
//
//            // Check, if user started to move with orbit camera with middle mouse button
//            const auto middleMouseButtonState = glfwGetMouseButton(w.getWindow(), GLFW_MOUSE_BUTTON_MIDDLE);
//            if(middleMouseButtonState == GLFW_PRESS)
//            {
//                if(!rotatingCamera && !movingCamera)
//                {
//                    movingCamera = true;
//                    glfwGetCursorPos(w.getWindow(), &prevMousePosX, &prevMousePosY);
//                }
//            } else if(middleMouseButtonState == GLFW_RELEASE)
//            {
//                movingCamera = false;
//            }
//
//            if(!(!rotatingCamera && !movingCamera))
//            {
//                // Only if we're rotating or moving we should calculate delta of mouse movement
//                glfwGetCursorPos(w.getWindow(), &curMousePosX, &curMousePosY);
//                const auto deltaX = static_cast<float>(curMousePosX - prevMousePosX);
//                const auto deltaY = static_cast<float>(curMousePosY - prevMousePosY);
//
//                if(rotatingCamera)
//                {
//                    w.getCamera().rotateAzimuth(deltaX * 0.01f);
//                    w.getCamera().rotatePolar(deltaY * 0.01f);
//                    prevMousePosX = curMousePosX;
//                    prevMousePosY = curMousePosY;
//                } else if(movingCamera)
//                {
//                    w.getCamera().moveHorizontal(-deltaX * 0.05f);
//                    w.getCamera().moveVertical(deltaY * 0.05f);
//                    prevMousePosX = curMousePosX;
//                    prevMousePosY = curMousePosY;
//                }
//            }
//            palka::RenderContext context1(&def, &ubo, palka::Mat4f{1.f}, [](palka::ShaderProgram& shader)
//            {});
//            palka::RenderContext context2(&material_light, &ubo, palka::Mat4f{1.f}, [](palka::ShaderProgram& shader)
//            {
//                shader.setUniform("material.ambient", ambient);
//                shader.setUniform("material.diffuse", diffuse);
//                shader.setUniform("material.specular", specular);
//                shader.setUniform("material.shininess", 35.f);
//
//                shader.setUniform("light.ambient", l_ambient);
//                shader.setUniform("light.diffuse", l_diffuse);
//                shader.setUniform("light.specular", l_specular);
//                shader.setUniform("light.position", palka::Vec3f{10, 0, 0});
//            });
//            w.drawLine(lx, context1);
//            w.drawLine(ly, context1);
//            w.drawLine(lz, context1);
//            w.draw(pp, context2, {1, 0, 1});
            //w.draw(m, context2, {1, 0, 1});


//            renderTex.bind();
//            renderTex.clear(palka::Color(0, 0, 0, 0));
//            renderTex.drawLine(lx, context1);
//            renderTex.drawLine(ly, context1);
//            renderTex.drawLine(lz, context1);
//            renderTex.draw(m, context2, {1, 0, 1});
//            renderTex.unbind();
//            ImGui::SetNextWindowPos(ImVec2((Config::WindowSize.x - (size.x)) / 2,
//                                           (Config::WindowSize.y - (size.y)) / 2), ImGuiCond_Always, {0, 0});
//            ImGui::Begin("View", NULL, ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove);
//            ImGui::SetWindowSize({size.x, size.y});
//            ImVec2 pos = ImGui::GetCursorScreenPos();
//            ImDrawList* drawList = ImGui::GetWindowDrawList();
//            unsigned int f_tex = renderTex.getTexture().textureID;
//            drawList->AddImage((void*) f_tex,
//                               pos,
//                               ImVec2(pos.x + 1280 * 0.85, pos.y + 720 * 0.85),
//                               ImVec2(0, 1),
//                               ImVec2(1, 0));
//            ImGui::End();
            manager.render(w);
            palka::Console::Draw("Console", &console_open);
            w.ImGuiEndFrame();
            w.EndFrame();
        }

        void update()
        {
            manager.update();
            timeSinceStart = glfwGetTime();
            delta = timeSinceStart - oldTimeSinceStart;
            oldTimeSinceStart = timeSinceStart;
            palka::EventManager::updateInputs();
        }

        void handleEvents()
        {
            w.eventHandler();
        }
    };
}

#endif //BILLET_APPLICATION_H
