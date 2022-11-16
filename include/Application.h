#ifndef BILLET_APPLICATION_H
#define BILLET_APPLICATION_H

#include <Window.h>
#include "Test.h"
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
        palka::ShaderProgram light;
        palka::ShaderProgram material_light;
        palka::ShaderProgram material_pbr;
        palka::assimp_loader test;
        palka::gltf_loader loader;
        tinygltf::Model mm;
        palka::Model gltf_model;
        palka::Model assimp_model;
        palka::VertArray cub;
        palka::VertexBufferObject vbo;
        palka::VertexArrayObject vao;
        double oldTimeSinceStart = 0;
        double timeSinceStart;
        float t = 0;
        float delta;
        SceneManager manager;
        IcoHolder icoHolder;

        palka::RenderTexture renderTex;
        palka::Sprite renderSp;
    public:
        explicit Application(palka::Vec2i size) : w(size), isRuning(false), view(palka::RectF(0, 0, size.x, size.y))
        , renderTex({500, 500})
        {
            init();
            m.init();
            ubo.create(sizeof(float[16]) * 3);

            palka::_Shader test1("Data\\Shaders\\PBR.frag", palka::_Shader::FRAGMENT);
            palka::_Shader test2("Data\\Shaders\\PBR.vert", palka::_Shader::VERTEX);
            p.createProgram();
            p.addShader(test1);
            p.addShader(test2);
            p.linkProgram();
            p.UBOBindingTo(p.getUBOIndex("matrixBuffer"), 0);
            // ubo.bindToPoint(0);

            palka::_Shader l1("Data\\Shaders\\light.frag", palka::_Shader::FRAGMENT);
            palka::_Shader l2("Data\\Shaders\\light.vert", palka::_Shader::VERTEX);

            light.createProgram();
            light.addShader(l1);
            light.addShader(l2);
            light.linkProgram();
            light.UBOBindingTo(p.getUBOIndex("matrixBuffer"), 0);
            // ubo.bindToPoint(0);

            palka::_Shader l3("Data\\Shaders\\material.frag", palka::_Shader::FRAGMENT);
            palka::_Shader l4("Data\\Shaders\\material.vert", palka::_Shader::VERTEX);
            material_light.createProgram();
            material_light.addShader(l3);
            material_light.addShader(l4);
            material_light.linkProgram();
            material_light.UBOBindingTo(material_light.getUBOIndex("matrixBuffer"), 0);

            palka::_Shader l5("Data\\Shaders\\PBR.frag", palka::_Shader::FRAGMENT);
            palka::_Shader l6("Data\\Shaders\\PBR.vert", palka::_Shader::VERTEX);
            material_pbr.createProgram();
            material_pbr.addShader(l6);
            material_pbr.addShader(l5);
            material_pbr.linkProgram();
            material_pbr.UBOBindingTo(material_pbr.getUBOIndex("matrixBuffer"), 0);

            ubo.bindToPoint(0);
            //assimp_model = test.load("Data\\model\\sphere.obj");
           // mm = loader.load("Data\\model\\glTF\\SciFiHelmet.gltf");
           // gltf_model = loader.bindModel(mm);
           // gltf_model.init();
            //  unsigned int rbo;

            renderTex.create();
            renderTex.getViewport().setCenter({500 / 2.f, 500 / 2.f});
            renderSp.setTexture(renderTex.getTexture());
            renderSp.setPosition({500, 0});
        }

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

            static palka::Vec3f lp = {-5.f, 0.f, -5.f};
            palka::Mat4f model3 = palka::Mat4f{1.f};
            model3 = glm::translate(model3, {0, 0, -5});
            static palka::Vec3f ambient = palka::Vec3f{1.0f, 0.5f, 0.31f};
            static palka::Vec3f diffuse = palka::Vec3f{1.0f, 0.5f, 0.31f};
            static palka::Vec3f specular = palka::Vec3f{0.5f, 0.5f, 0.5f};
            static palka::Vec3f l_ambient = palka::Vec3f{0.2f, 0.2f, 0.2f};
            static palka::Vec3f l_diffuse = palka::Vec3f{0.5f, 0.5f, 0.5f};
            static palka::Vec3f l_specular = palka::Vec3f{1.0f, 1.0f, 1.0f};
            static float shininess = 32.f;


            palka::RenderContext context2(&material_light, &ubo, model3, [](palka::ShaderProgram& shader)
            {
                shader.setUniform("material.ambient", ambient);
                shader.setUniform("material.diffuse", diffuse);
                shader.setUniform("material.specular", specular);
                shader.setUniform("material.shininess", shininess);

                shader.setUniform("light.ambient", l_ambient);
                shader.setUniform("light.diffuse", l_diffuse);
                shader.setUniform("light.specular", l_specular);
                shader.setUniform("light.position", lp);
            });

            renderTex.bind();
            renderTex.clear(palka::Color(0, 0, 0, 0));
            renderTex.draw(m, context2, lp);
            renderTex.unbind();

            ImGui::Begin("Game Window");
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            unsigned int f_tex = renderTex.getTexture().textureID;
            drawList->AddImage((void*)f_tex,
                               pos,
                               ImVec2(pos.x + 512, pos.y + 512),
                               ImVec2(0, 1),
                               ImVec2(1, 0));
            ImGui::End();

            //  manager.getScene()->render();
            palka::Console::Draw("Console", &console_open);
            w.ImGuiEndFrame();
            w.EndFrame();
        }

        void update()
        {
            manager.getScene()->update();
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
