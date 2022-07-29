//
// Created by NePutin on 1/22/2022.
//

#ifndef BILLET_APPLICATION_H
#define BILLET_APPLICATION_H

#include "Window.h"
#include "VertexBuffer.h"
#include "ReflectionDebug.h"
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
        palka::VertArray cub;
        VertexBuffer buff;
        // Shader vertexShader;
        // Shader indexShader;
        // Shader light;
        VertexBufferObject vbo;
        VertexArrayObject vao;
        UniformBuffer ubo;

        StaticMesh m;
        ShaderProgram p;
        ShaderProgram light;
        ShaderProgram material_light;
        ShaderProgram material_pbr;
//        assimp_loader test;
//        gltf_loader loader;
//        tinygltf::Model mm;
//        Model gltf_model;
//        Model assimp_model;
        //VertexArrayObject vvao;
        //unsigned int vbo_;
        //unsigned int vao_;
        MainScene scene;

       // DockWrapper dockw;

    public:
        explicit Application(Vec2i size) : w(size), isRuning(false), view(RectF(0, 0, size.x, size.y))
        {
            init();
            //dockw.addWindow(DockWrapper::Left, &test);
            scene.init();
//            w.getViewport().setCenter({1280.f / 2, 720.f / 2});
//            m.init();
//            cub.add({{-0.5f, -0.5f, -0.5f,}, Color{255, 0, 0}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}});
//            cub.add({{0.5f, -0.5f, -0.5f,}, Color{255, 0, 0}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}});
//            cub.add({{0.5f, 0.5f, -0.5f,}, Color{255, 0, 0}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}});
//            cub.add({{0.5f, 0.5f, -0.5f,}, Color{255, 0, 0}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f}});
//            cub.add({{-0.5f, 0.5f, -0.5f,}, Color{255, 0, 0}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f}});
//            cub.add({{-0.5f, -0.5f, -0.5f,}, Color{255, 0, 0}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f}});
//            cub.add({{-0.5f, -0.5f, 0.5f,}, Color{255, 0, 0}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});
//            cub.add({{0.5f, -0.5f, 0.5f,}, Color{255, 0, 0}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});
//            cub.add({{0.5f, 0.5f, 0.5f,}, Color{255, 0, 0}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});
//            cub.add({{0.5f, 0.5f, 0.5f,}, Color{255, 0, 0}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});
//            cub.add({{-0.5f, 0.5f, 0.5f,}, Color{255, 0, 0}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}});
//            cub.add({{-0.5f, -0.5f, 0.5f,}, Color{255, 0, 0}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}});
//            cub.add({{-0.5f, 0.5f, 0.5f,}, Color{255, 0, 0}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}});
//            cub.add({{-0.5f, 0.5f, -0.5f,}, Color{255, 0, 0}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}});
//            cub.add({{-0.5f, -0.5f, -0.5f,}, Color{255, 0, 0}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}});
//            cub.add({{-0.5f, -0.5f, -0.5f,}, Color{255, 0, 0}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}});
//            cub.add({{-0.5f, -0.5f, 0.5f,}, Color{255, 0, 0}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}});
//            cub.add({{-0.5f, 0.5f, 0.5f,}, Color{255, 0, 0}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}});
//            cub.add({{0.5f, 0.5f, 0.5f,}, Color{255, 0, 0}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
//            cub.add({{0.5f, 0.5f, -0.5f,}, Color{255, 0, 0}, {1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});
//            cub.add({{0.5f, -0.5f, -0.5f,}, Color{255, 0, 0}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});
//            cub.add({{0.5f, -0.5f, -0.5f,}, Color{255, 0, 0}, {0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});
//            cub.add({{0.5f, -0.5f, 0.5f,}, Color{255, 0, 0}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
//            cub.add({{0.5f, 0.5f, 0.5f,}, Color{255, 0, 0}, {1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
//            cub.add({{-0.5f, -0.5f, -0.5f,}, Color{255, 0, 0}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}});
//            cub.add({{0.5f, -0.5f, -0.5f,}, Color{255, 0, 0}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f}});
//            cub.add({{0.5f, -0.5f, 0.5f,}, Color{255, 0, 0}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}});
//            cub.add({{0.5f, -0.5f, 0.5f,}, Color{255, 0, 0}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}});
//            cub.add({{-0.5f, -0.5f, 0.5f,}, Color{255, 0, 0}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f}});
//            cub.add({{-0.5f, -0.5f, -0.5f,}, Color{255, 0, 0}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f}});
//            cub.add({{-0.5f, 0.5f, -0.5f,}, Color{255, 0, 0}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}});
//            cub.add({{0.5f, 0.5f, -0.5f,}, Color{255, 0, 0}, {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}});
//            cub.add({{0.5f, 0.5f, 0.5f,}, Color{255, 0, 0}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}});
//            cub.add({{0.5f, 0.5f, 0.5f}, Color{255, 0, 0}, {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}});
//            cub.add({{-0.5f, 0.5f, 0.5f}, Color{255, 0, 0}, {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}});
//            cub.add({{-0.5f, 0.5f, -0.5f,}, Color{255, 0, 0}, {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}});
//
//            ubo.create(sizeof(float[16]) * 3);
//
//            _Shader test1("Data\\Shaders\\PBR.frag", _Shader::FRAGMENT);
//            _Shader test2("Data\\Shaders\\PBR.vert", _Shader::VERTEX);
//            p.createProgram();
//            p.addShader(test1);
//            p.addShader(test2);
//            p.linkProgram();
//            p.UBOBindingTo(p.getUBOIndex("matrixBuffer"), 0);
//            // ubo.bindToPoint(0);
//
//            _Shader l1("Data\\Shaders\\light.frag", _Shader::FRAGMENT);
//            _Shader l2("Data\\Shaders\\light.vert", _Shader::VERTEX);
//
//            light.createProgram();
//            light.addShader(l1);
//            light.addShader(l2);
//            light.linkProgram();
//            light.UBOBindingTo(p.getUBOIndex("matrixBuffer"), 0);
//            // ubo.bindToPoint(0);
//
//            _Shader l3("Data\\Shaders\\material.frag", _Shader::FRAGMENT);
//            _Shader l4("Data\\Shaders\\material.vert", _Shader::VERTEX);
//            material_light.createProgram();
//            material_light.addShader(l3);
//            material_light.addShader(l4);
//            material_light.linkProgram();
//            material_light.UBOBindingTo(material_light.getUBOIndex("matrixBuffer"), 0);
//
//            _Shader l5("Data\\Shaders\\PBR.frag", _Shader::FRAGMENT);
//            _Shader l6("Data\\Shaders\\PBR.vert", _Shader::VERTEX);
//            material_pbr.createProgram();
//            material_pbr.addShader(l6);
//            material_pbr.addShader(l5);
//            material_pbr.linkProgram();
//            material_pbr.UBOBindingTo(material_pbr.getUBOIndex("matrixBuffer"), 0);
//
//            ubo.bindToPoint(0);
//
////            glCreateBuffers(1, &vbo_);
////            glNamedBufferStorage(vbo_, sizeof(Vertex) * cub.getSize(), &cub[0].pos.x, GL_DYNAMIC_STORAGE_BIT);
////
////
////            glCreateVertexArrays(1, &vao_);
////
////            glVertexArrayVertexBuffer(vao_, 0, vbo_, 0, sizeof(Vertex));
////
////            glEnableVertexArrayAttrib(vao_, 0);
////            glEnableVertexArrayAttrib(vao_, 2);
////            glEnableVertexArrayAttrib(vao_, 3);
////
////            glVertexArrayAttribFormat(vao_, 0, 3, GL_FLOAT, GL_FALSE, 0);
////            glVertexArrayAttribFormat(vao_, 2, 2, GL_FLOAT, GL_FALSE, 32);
////            glVertexArrayAttribFormat(vao_, 3, 3, GL_FLOAT, GL_FALSE, 40);
////
////            glVertexArrayAttribBinding(vao_, 0, 0);
////            glVertexArrayAttribBinding(vao_, 2, 0);
////            glVertexArrayAttribBinding(vao_, 3, 0);
//
//            vao.create(cub.getSize());
//            vbo.create(sizeof(Vertex) * cub.getSize());
//            vbo.setData(sizeof(Vertex) * cub.getSize(), &cub[0].pos.x);
//            //vao.linkVBO(vbo, sizeof(Vertex));
//            //vao.setPointer_dsa(vbo, 0, 3, GL_FLOAT, 0, false);
//            vao.setPointers(vbo, sizeof(Vertex));

        }


        void mainApp()
        {
            //dockw.render();
            ImGui::ShowDemoWindow();
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
            //w.setViewport(view);
            isRuning = true;
            EventManager::addEvent(KBoardEvent::KeyPressed(GLFW_KEY_GRAVE_ACCENT), [this](EventData e)
            {
                console_open = !console_open;
            });
            EventManager::addEvent(EventType::WINDOWCLOSE, [this](EventData e)
            {
                isRuning = false;
            });
            //w.init();
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
