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
        palka::Model gltf_model;
        palka::Model assimp_model;
        palka::PlaneMesh plane;
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
        explicit Application(palka::Vec2i size) : w(size), isRuning(false), view(palka::RectF(0, 0, size.x, size.y)), renderTex({500, 500})
        {
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
            lx.init();
            ly.init();
            lz.init();
            auto pp = calcPlaneCoord(1, 1, 1, 0);
            plane.init(pp[0], pp[1], pp[2]);//
        }

        std::array<palka::Vec3f, 6> calcPlaneCoord(float a, float b, float c, float d)
        {
            std::array<palka::Vec3f, 6> res{};
            palka::Vec3f vec{0};

            //ax + by + cz = d
            //z = (d - ax - by) / c
            //y = (d - ax - cz) / b
            //x = (d - by - cz) / a

            auto calcY = [=](float p1, float p2)
            { return (d - a * p1 - b * p2) / c; };
            auto calcZ = [=](float p1, float p2)
            { return (d - a * p1 - c * p2) / b; };
            auto calcX = [=](float p1, float p2)
            { return (d - b * p1 - c * p2) / a; };

            if(a == 0 || b == 0 || c == 0)
            {
                if(a == 0 && b != 0 && c != 0)
                {
                    res[0] = {1, 0, 0};
                    res[1] = {-1, 0, 0};
                    res[2] = {0, 1, calcZ(0, 1)};
                    return res;
                } else if(a == 0 && b != 0)
                {
                    res[0] = {1, 0, 0};
                    res[1] = {-1, 0, 0};
                    res[2] = {0, 1, 0};
                    return res;
                } else if(a == 0 && c != 0)
                {
                    res[0] = {1, 0, 0};
                    res[1] = {-1, 0, 0};
                    res[2] = {0, 0, 1};
                    return res;
                }


                if(b == 0 && a != 0 && c != 0)
                {
                    res[0] = {0, 0, c};
                    res[1] = {0, 0, -c};
                    res[2] = {calcX(0, 1), 1, 0};
                    return res;
                } else if(b == 0 && a != 0)
                {
                    res[0] = {0, 0, -1};
                    res[1] = {0, 0, 1};
                    res[2] = {0, 1, 0};
                    return res;
                }

                if(c == 0 && b != 0 && a != 0)
                {
                    res[0] = {calcX(-1, 0), 0, -1};
                    res[1] = {calcX(1, 0), 0, 1};
                    res[2] = {0, 1, 0};
                    return res;
                }
            } else
            {
                res[0] = {calcX(1, 0), 0, 1};
                res[1] = {1, 0, calcY(1, 0)};
                res[2] = {calcX(0, 1), 1, 0};
                return res;
            }
            return res;
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

        glm::quat RotationBetweenVectors(glm::vec3 start, glm::vec3 dest)
        {
            start = normalize(start);
            dest = normalize(dest);

            float cosTheta = dot(start, dest);
            glm::vec3 rotationAxis;

            if(cosTheta < -1 + 0.001f)
            {
                // special case when vectors in opposite directions:
                // there is no "ideal" rotation axis
                // So guess one; any will do as long as it's perpendicular to start
                rotationAxis = cross(glm::vec3(0.0f, 0.0f, 1.0f), start);
                if(glm::length2(rotationAxis) < 0.01) // bad luck, they were parallel, try again!
                    rotationAxis = cross(glm::vec3(1.0f, 0.0f, 0.0f), start);

                rotationAxis = normalize(rotationAxis);
                return glm::angleAxis(glm::radians(180.0f), rotationAxis);
            }

            rotationAxis = cross(start, dest);

            float s = sqrt((1 + cosTheta) * 2);
            float invs = 1 / s;

            return glm::quat(
                    s * 0.5f,
                    rotationAxis.x * invs,
                    rotationAxis.y * invs,
                    rotationAxis.z * invs
            );

        }

        void render()
        {
            static bool update = false;
            glm::vec3 xNorm(1.0, 0.0f, 0.0);
            glm::vec3 yNorm(0.0, 1.0f, 0.0);
            glm::vec3 zNorm(0.0, 0.0f, 1.0);
            static palka::Vec3f lp = {-5.f, 0.f, -5.f};
            static float degrees = 0.f;
            static float degreesY = 0.f;
            static float degreesZ = 0.f;
            static float posX = 0.f;
            static float posY = 0.f;
            static float posZ = 0.f;
            static int x = 1;
            static int y = 1;
            static int z = 0;

            float abcd[4] = {1, 0, 0, 0};
            abcd[0] = x;
            abcd[1] = y;
            abcd[2] = z;
            glm::vec3 myPlaneNormal = {1, 0, 0};
            glm::vec3 planeNormal{abcd[0], abcd[2], abcd[1]};

            auto RotAxis = glm::cross(myPlaneNormal, planeNormal);
            auto RotAngle = acos(glm::dot(myPlaneNormal, planeNormal) / (glm::length(myPlaneNormal) * glm::length(planeNormal)));

            auto quat = RotationBetweenVectors(myPlaneNormal, planeNormal);

            auto angle1 = std::acos(std::abs(abcd[2]) / sqrt(abcd[0] * abcd[0] + abcd[1] * abcd[1] + abcd[2] * abcd[2]));
            auto angle2 = std::acos(std::abs(abcd[0]) / sqrt(abcd[0] * abcd[0] + abcd[1] * abcd[1] + abcd[2] * abcd[2]));
            auto angle3 = std::acos(std::abs(abcd[1]) / sqrt(abcd[0] * abcd[0] + abcd[1] * abcd[1] + abcd[2] * abcd[2]));
//            auto angleX = glm::orientedAngle(planeNormal, xNorm, myPlaneNormal);
//            auto angleY = glm::orientedAngle(planeNormal, yNorm, myPlaneNormal);
//            auto angleZ = glm::orientedAngle(planeNormal, zNorm, myPlaneNormal);
            auto len = sqrt(abcd[0] * abcd[0] + abcd[1] * abcd[1] + abcd[2] * abcd[2]);
            float angleX = glm::acos(glm::dot(glm::vec3{abcd[0], abcd[1], abcd[2]}, xNorm) / len);
            float angleY = glm::acos(glm::dot(glm::vec3{abcd[0], abcd[1], abcd[2]}, yNorm) / len);
            float angleZ = glm::acos(glm::dot(glm::vec3{abcd[0], abcd[1], abcd[2]}, zNorm) / len);
            w.clear();

            ImGui::DragFloat("angleX", &degrees);
            ImGui::DragFloat("angleY", &degreesY);
            ImGui::DragFloat("angleZ", &degreesZ);
//            ImGui::DragFloat("posX", &posX);
//            ImGui::DragFloat("posY", &posY);
//            ImGui::DragFloat("posZ", &posZ);
            if(ImGui::DragInt("x", &x)) update = true;
            if(ImGui::DragInt("y", &y)) update = true;
            if(ImGui::DragInt("z", &z)) update = true;

            if(update)
            {
                auto pp = calcPlaneCoord(x, y, z, 0);
                plane.update(pp[0], pp[1], pp[2]);
                update = false;
            }
            auto qx = glm::angleAxis(angleX, xNorm);
            auto qy = glm::angleAxis(glm::radians(angleY * (180.f / 3.14f) - 90.f), yNorm);
            auto qz = glm::angleAxis(glm::radians(angleZ * (180.f / 3.14f) - 90.f), zNorm);
            glm::mat4 RotationMatrix = glm::toMat4(quat);

            palka::Mat4f model3 = palka::Mat4f{1.f};
            // model3 = glm::rotate(model3, RotAngle, RotAxis);
            //model3 = RotationMatrix;
//            palka::Mat4f rotX = palka::Mat4f{1.f};
//            palka::Mat4f rotY = palka::Mat4f{1.f};
//            palka::Mat4f rotZ = palka::Mat4f{1.f};
//            rotX = glm::rotate(rotX, angleX, xNorm);
//            rotY = glm::rotate(rotY, angleY, yNorm);
//            rotZ = glm::rotate(rotZ, angleZ, zNorm);
            //model3 = rotX * rotY * rotZ;


//            model3 = glm::translate(model3, {posX, posY, posZ});
            //           model3 = glm::rotate(model3, glm::radians(degrees), {1, 0, 0});
            //          model3 = glm::rotate(model3, glm::radians(degreesY), {0, 1, 0});
            //           model3 = glm::rotate(model3, glm::radians(degreesZ), {0, 0, 1});

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
            palka::RenderContext context1(&def, &ubo, palka::Mat4f{1.f}, [](palka::ShaderProgram& shader)
            {});

            w.drawLine(lx, context1);
            w.drawLine(ly, context1);
            w.drawLine(lz, context1);

            w.draw(plane, context2, lp);
//            w.draw(m, context2, lp);
//            renderTex.bind();
//            renderTex.clear(palka::Color(0, 0, 0, 0));
//            renderTex.draw(m, context2, lp);
//            renderTex.unbind();
//
//            ImGui::Begin("Game Window");
//            ImVec2 pos = ImGui::GetCursorScreenPos();
//            ImDrawList* drawList = ImGui::GetWindowDrawList();
//            unsigned int f_tex = renderTex.getTexture().textureID;
//            drawList->AddImage((void*)f_tex,
//                               pos,
//                               ImVec2(pos.x + 512, pos.y + 512),
//                               ImVec2(0, 1),
//                               ImVec2(1, 0));
//            ImGui::End();

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
