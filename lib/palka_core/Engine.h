////
//// Created by NePutin on 6/23/2021.
////
//
//#ifndef PALKA_ENGINE_H
//#define PALKA_ENGINE_H
//
//#include <string>
//#include <glm/vec2.hpp>
//#include <glm/vec3.hpp>
//#include "Vec2.h"
//#include "ConsoleLog.h"
//#include "Window.h"
//#include "EventManager.h"
//#include "ReflectionDebug.h"
//#include "Texture.h"
//#include "Shader.h"
//#include "Sprite.h"
//#include "VertexBuffer.h"
//#include "VertexBufferObject.h"
//#include "VertexArrayObject.h"
//#include "StaticMesh.h"
//#include "ShaderProgram.h"
//#include "Mesh.h"
//
//
//namespace palka
//{
//    class Engine
//    {
//    private:
//        bool console_open = false;
//        Viewport view;
//        Window w;
//
//        bool isRuning = true;
//
//        double oldTimeSinceStart = 0;
//        double timeSinceStart;
//        float t = 0;
//        float delta;
//        palka::VertArray cub;
//        VertexBuffer buff;
//        // Shader vertexShader;
//        // Shader indexShader;
//        // Shader light;
//        VertexBufferObject vbo;
//        VertexArrayObject vao;
//        UniformBuffer ubo;
//
//        StaticMesh m;
//        ShaderProgram p;
//        ShaderProgram light;
//        ShaderProgram material_light;
//        ShaderProgram material_pbr;
//        assimp_loader test;
//        gltf_loader loader;
//        tinygltf::Model mm;
//        Model gltf_model;
//        Model assimp_model;
//        //VertexArrayObject vvao;
//        //unsigned int vbo_;
//        //unsigned int vao_;
//    public:
//        explicit Engine(Vec2i size) : w(size), isRuning(false), view(RectF(0, 0, size.x, size.y))
//        {
//            init();
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
//
//            assimp_model = test.load("Data\\model\\sphere.obj");
//            mm = loader.load("Data\\model\\glTF\\SciFiHelmet.gltf");
//            gltf_model = loader.bindModel(mm);
//            gltf_model.init();
//            //vvao = std::move(loader.setVAO());
//        }
//
//        void run()
//        {
//            glfwSetTime(0);
//            while(isRuning)
//            {
//                handleEvents();
//                w.ImGUiNewFrame();
//                update();
//                render();
//            }
//        }
//
//        void init()
//        {
//            w.create();
//            //w.setViewport(view);
//            isRuning = true;
//            EventManager::addEvent(KBoardEvent::KeyPressed(GLFW_KEY_GRAVE_ACCENT), [this](EventData e)
//            {
//                console_open = !console_open;
//            });
//            EventManager::addEvent(EventType::WINDOWCLOSE, [this](EventData e)
//            {
//                isRuning = false;
//            });
//            //w.init();
//        }
//
//        void render()
//        {
//            w.clear();
//
//            static Vec3f lp = {-5.f, 0.f, -5.f};
//            Mat4f lightPos = Mat4f{1.f};
//            ImGui::DragFloat("x", &lp.x);
//            ImGui::DragFloat("y", &lp.y);
//            ImGui::DragFloat("z", &lp.z);
//            lightPos = glm::translate(lightPos, lp);
//
//            Mat4f model22 = Mat4f{1.f};
//            model22 = glm::translate(model22, {8, 0, 0});
//            model22 = glm::rotate(model22, 130.f, {0, 1, 0});
//            RenderContext context12(&p, &ubo, model22, [](ShaderProgram& shader)
//            {
//                shader.setUniform("objectColor", Vec3f{0.2f, 0.1f, 0.9f});
//                shader.setUniform("lightColor", Vec3f{1.f, 1.0f, 1.0f});
//                shader.setUniform("lightPos", lp);
//
//            });
//            w.draw(gltf_model, context12);
//
//            Mat4f model = Mat4f{1.f};
//            model = glm::translate(model, {-15, 0, 0});
//            RenderContext context1(&light, &ubo, model);
//            w.draw(vao, context1, lp);
//
//
//            static Vec3f ambient = Vec3f{1.0f, 0.5f, 0.31f};
//            static Vec3f diffuse = Vec3f{1.0f, 0.5f, 0.31f};
//            static Vec3f specular = Vec3f{0.5f, 0.5f, 0.5f};
//            static Vec3f l_ambient = Vec3f{0.2f, 0.2f, 0.2f};
//            static Vec3f l_diffuse = Vec3f{0.5f, 0.5f, 0.5f};
//            static Vec3f l_specular = Vec3f{1.0f, 1.0f, 1.0f};
//            static float shininess = 32.f;
//            {
//                ImGui::DragFloat("l_ambient.x", &l_ambient.x);
//                ImGui::DragFloat("l_ambient.y", &l_ambient.y);
//                ImGui::DragFloat("l_ambient.z", &l_ambient.z);
//            }
//            {
//                ImGui::DragFloat("l_diffuse.x", &l_diffuse.x);
//                ImGui::DragFloat("l_diffuse.y", &l_diffuse.y);
//                ImGui::DragFloat("l_diffuse.z", &l_diffuse.z);
//            }
//            {
//                ImGui::DragFloat("l_specular.x", &l_specular.x);
//                ImGui::DragFloat("l_specular.y", &l_specular.y);
//                ImGui::DragFloat("l_specular.z", &l_specular.z);
//            }
//            {
//                ImGui::DragFloat("ambient.x", &ambient.x);
//                ImGui::DragFloat("ambient.y", &ambient.y);
//                ImGui::DragFloat("ambient.z", &ambient.z);
//            }
//            {
//                ImGui::DragFloat("diffuse.x", &diffuse.x);
//                ImGui::DragFloat("diffuse.y", &diffuse.y);
//                ImGui::DragFloat("diffuse.z", &diffuse.z);
//            }
//            {
//                ImGui::DragFloat("specular.x", &specular.x);
//                ImGui::DragFloat("specular.y", &specular.y);
//                ImGui::DragFloat("specular.z", &specular.z);
//            }
//            {
//                ImGui::DragFloat("shininess", &shininess);
//            }
//            Mat4f model3 = Mat4f{1.f};
//            model3 = glm::translate(model3, {-8, 0, 0});
//            RenderContext context2(&material_light, &ubo, model3, [](ShaderProgram& shader)
//            {
//                shader.setUniform("material.ambient", ambient);
//                shader.setUniform("material.diffuse", diffuse);
//                shader.setUniform("material.specular", specular);
//                shader.setUniform("material.shininess", shininess);
//
//                shader.setUniform("light.ambient", l_ambient);
//                shader.setUniform("light.diffuse", l_diffuse);
//                shader.setUniform("light.specular", l_specular);
//                shader.setUniform("light.position", lp);
//            });
//            w.draw(m, context2, lp);
//
//            RenderContext context3(&light, &ubo, lightPos);
//            w.draw(vao, context3, lp);
//
//            Mat4f model2 = Mat4f{1.f};
//            model2 = glm::translate(model2, {0, 0, 0});
//            RenderContext context4(&p, &ubo, model2, [](ShaderProgram& shader)
//            {
//                shader.setUniform("objectColor", Vec3f{0.2f, 0.1f, 0.9f});
//                shader.setUniform("lightColor", Vec3f{1.f, 1.0f, 1.0f});
//                shader.setUniform("lightPos", lp);
//
//            });
//            w.draw(assimp_model, context4);
//
//
//            Console::Draw("Console", &console_open);
//            w.ImGuiEndFrame();
//            w.EndFrame();
//        }
//
//        void update()
//        {
//            debug(w.getCamera(), "camera");
//            debug(w.getViewport(), "view");
//            timeSinceStart = glfwGetTime();
//            delta = timeSinceStart - oldTimeSinceStart;
//            oldTimeSinceStart = timeSinceStart;
//            EventManager::updateInputs();
//        }
//
//        void handleEvents()
//        {
//            w.eventHandler();
//        }
//    };
//}
//
//#endif //PALKA_ENGINE_H
