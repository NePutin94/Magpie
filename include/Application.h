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
        palka::PlaneMesh plane2;
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
//            static bool update = false;
//            static palka::Vec3f lp = {-5.f, 0.f, -5.f};
//            static float posX = 0.f;
//            static float posY = 0.f;
//            static float posZ = 0.f;
//            static int x = 1;
//            static int y = 1;
//            static int z = 0;
//
//            float abcd[4] = {1, 0, 0, 0};
//            abcd[0] = x;
//            abcd[1] = y;
//            abcd[2] = z;
//            glm::vec3 myPlaneNormal = {1, 0, 0};
//            glm::vec3 planeNormal{abcd[0], abcd[2], abcd[1]};
//
//            auto RotAxis = glm::cross(myPlaneNormal, planeNormal);
//            auto RotAngle = acos(glm::dot(myPlaneNormal, planeNormal) / (glm::length(myPlaneNormal) * glm::length(planeNormal)));
            w.clear();

//            if(ImGui::DragInt("x", &x)) update = true;
//            if(ImGui::DragInt("y", &y)) update = true;
//            if(ImGui::DragInt("z", &z)) update = true;
//
//            palka::Mat4f model3 = palka::Mat4f{1.f};
//            model3 = glm::rotate(model3, RotAngle, RotAxis);
//
//            static palka::Vec3f ambient = palka::Vec3f{1.0f, 0.5f, 0.31f};
//            static palka::Vec3f diffuse = palka::Vec3f{1.0f, 0.5f, 0.31f};
//            static palka::Vec3f specular = palka::Vec3f{0.5f, 0.5f, 0.5f};
//            static palka::Vec3f l_ambient = palka::Vec3f{0.2f, 0.2f, 0.2f};
//            static palka::Vec3f l_diffuse = palka::Vec3f{0.5f, 0.5f, 0.5f};
//            static palka::Vec3f l_specular = palka::Vec3f{1.0f, 1.0f, 1.0f};
//            static float shininess = 32.f;
//
//
//            palka::RenderContext context2(&material_light, &ubo, model3, [](palka::ShaderProgram& shader)
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
//            palka::RenderContext context1(&def, &ubo, palka::Mat4f{1.f}, [](palka::ShaderProgram& shader)
//            {});
//
//            w.drawLine(lx, context1);
//            w.drawLine(ly, context1);
//            w.drawLine(lz, context1);
//
//            glEnable(GL_CLIP_DISTANCE0);
//            w.draw(plane, context2, lp);
//
//
//            glm::vec3 myPlaneNormal2 = {1, 0, 0};
//            glm::vec3 planeNormal2{0, 1, 0};
//
//            auto RotAxis2 = glm::cross(myPlaneNormal2, planeNormal2);
//            auto RotAngle2 = acos(glm::dot(myPlaneNormal2, planeNormal2) / (glm::length(myPlaneNormal2) * glm::length(planeNormal2)));
//            palka::Mat4f model4 = palka::Mat4f{1.f};
//            model4 = glm::rotate(model4, RotAngle2, RotAxis2);
//
//            float PlaneEquation[] = {1, 1, 0, 0};
//            palka::RenderContext context3(&material_light, &ubo, model4, [&](palka::ShaderProgram& shader)
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
//                shader.setUniform("ClipPlane", &PlaneEquation[0], 4);
//            });
//
//            w.draw(plane2, context3, lp);
//
//            glDisable(GL_CLIP_PLANE0);
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

            manager.getScene()->render(w);
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
