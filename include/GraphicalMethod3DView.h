#ifndef MAGPIE_GRAPHICALMETHOD3DVIEW_H
#define MAGPIE_GRAPHICALMETHOD3DVIEW_H

#include "UiView.h"
#include "GraphicsMethod2D.h"
#include "config.h"
#include "myImGui.h"
#include <glm/matrix.hpp>
#include "SimplexMethod.h"
#include "RenderTexture.h"
#include "Sprite.h"
#include <implot.h>
#include <utility>
#include <imgui.h>
#include <imgui_internal.h>
#include <ReflectionDebug.h>
#include "PolygonMesh.h"
#include "GraphicsMethod3D.h"

namespace Magpie
{
    template<class T>
    class GraphicalMethod3DView : public UiView
    {
    private:
        GraphicMet3D<T> solver3d;

        float delta = 0;
        palka::Vec2<double> vec;

        ImGui::FileManager_Context c;

        //3D staff//
        palka::RenderTexture renderTex;
        palka::RenderTexture screenTexture;
        palka::ShaderProgram material_anim;
        palka::ShaderProgram material;
        palka::ShaderProgram normals;
        palka::ShaderProgram def;
        palka::ShaderProgram outline;
        palka::UniformBuffer ubo;
        palka::SphereMesh sphere;
        palka::Renderer::Line lx{{-10, 0, 0},
                                 {10, 0, 0},
                                 palka::Color(255, 0, 0)};
        palka::Renderer::Line ly{{0, -10, 0},
                                 {0, 10, 0},
                                 palka::Color(0, 255, 0)};
        palka::Renderer::Line lz{{0, 0, -10},
                                 {0, 0, 10},
                                 palka::Color(0, 0, 255)};
        palka::Renderer::Line targetPlaneNormal;
        palka::PolygonMesh polygon;
        palka::PlaneMesh plane;

        //target plane trasnform
        palka::Mat4f plane_trasnform{1.f};
        palka::Vec3f scale{1.f};

        void init3D()
        {
            ubo.create(sizeof(float[16]) * 3);

            palka::_Shader def1("Data\\Shaders\\Default.frag", palka::_Shader::FRAGMENT);
            palka::_Shader def2("Data\\Shaders\\Default.vert", palka::_Shader::VERTEX);
            def.createProgram();
            def.addShader(def1);
            def.addShader(def2);
            def.linkProgram();
            def.UBOBindingTo(def.getUBOIndex("matrixBuffer"), 0);

            palka::_Shader l3("Data\\Shaders\\explode.frag", palka::_Shader::FRAGMENT);
            palka::_Shader l4("Data\\Shaders\\explode.vert", palka::_Shader::VERTEX);
            palka::_Shader l5("Data\\Shaders\\explode.gs", palka::_Shader::GEOMETRY);
            material_anim.createProgram();
            material_anim.addShader(l3);
            material_anim.addShader(l4);
            material_anim.addShader(l5);
            material_anim.linkProgram();
            material_anim.UBOBindingTo(material_anim.getUBOIndex("matrixBuffer"), 0);

            palka::_Shader l9("Data\\Shaders\\material.frag", palka::_Shader::FRAGMENT);
            palka::_Shader l10("Data\\Shaders\\material.vert", palka::_Shader::VERTEX);
            material.createProgram();
            material.addShader(l9);
            material.addShader(l10);
            material.linkProgram();
            material.UBOBindingTo(material.getUBOIndex("matrixBuffer"), 0);

            palka::_Shader l6("Data\\Shaders\\normals.frag", palka::_Shader::FRAGMENT);
            palka::_Shader l7("Data\\Shaders\\normals.vert", palka::_Shader::VERTEX);
            palka::_Shader l8("Data\\Shaders\\normals.gs", palka::_Shader::GEOMETRY);
            normals.createProgram();
            normals.addShader(l6);
            normals.addShader(l7);
            normals.addShader(l8);
            normals.linkProgram();
            normals.UBOBindingTo(normals.getUBOIndex("matrixBuffer"), 0);

            palka::_Shader outlineFrag("Data\\Shaders\\Outline.frag", palka::_Shader::FRAGMENT);
            palka::_Shader outlineVert("Data\\Shaders\\Outline.vert", palka::_Shader::VERTEX);
            outline.createProgram();
            outline.addShader(outlineFrag);
            outline.addShader(outlineVert);
            outline.linkProgram();
            outline.UBOBindingTo(outline.getUBOIndex("matrixBuffer"), 0);

            ubo.bindToPoint(0);
            lx.init();
            ly.init();
            lz.init();
            renderTex.create();
            renderTex.getViewport().setCenter({1920.f / 2.f, 1080.f / 2.f});

            screenTexture.create();
            screenTexture.getViewport().setCenter({1920.f / 2.f, 1080.f / 2.f});

            plane.init();
            sphere.init();
        }

    public:
        GraphicalMethod3DView(palka::Vec2f pos, palka::Vec2f size, MatrixStorage<T> input, bool open = true,
                              ImGuiWindowFlags w_flag = ImGuiWindowFlags_None)
                : UiView("GraphicalMethodView", pos, size, open, w_flag), c("./", true), renderTex({1920.f, 1080.f}),
                  screenTexture({1920.f, 1080.f}), sphere(0.1, 20, 20)
        {
            init3D();
        }

        GraphicalMethod3DView(std::string name, palka::Vec2f size)
                : UiView(name, size), c("./", true),
                  renderTex({1920.f, 1080.f}),
                  screenTexture({1920.f, 1080.f}),
                  sphere(0.1, 20, 20)
        {
            init3D();
        }

        GraphicalMethod3DView(GraphicalMethod3DView&& ot) = default;

        void outlineRenderPass(palka::Window& w)
        {
            static palka::Vec3f ambient = palka::Vec3f{1.0f, 0.5f, 0.31f};
            static palka::Vec3f diffuse = palka::Vec3f{1.0f, 0.5f, 0.31f};
            static palka::Vec3f specular = palka::Vec3f{0.5f, 0.5f, 0.5f};
            static palka::Vec3f l_ambient = palka::Vec3f{0.2f, 0.2f, 0.2f};
            static palka::Vec3f l_diffuse = palka::Vec3f{0.5f, 0.5f, 0.5f};
            static palka::Vec3f l_specular = palka::Vec3f{1.0f, 1.0f, 1.0f};
            screenTexture.getCamera().updateCamera(w.getWindow());

            palka::RenderContext context2(&material, &ubo, palka::Mat4f{1.f}, [](palka::ShaderProgram& shader)
            {
                shader.setUniform("material.ambient", ambient);
                shader.setUniform("material.diffuse", diffuse);
                shader.setUniform("material.specular", specular);
                shader.setUniform("material.shininess", 35.f);

                shader.setUniform("light.ambient", l_ambient);
                shader.setUniform("light.diffuse", l_diffuse);
                shader.setUniform("light.specular", l_specular);
                shader.setUniform("light.position", palka::Vec3f{10, 0, 0});
            });
            screenTexture.bind();
            screenTexture.clear(palka::Color(0, 0, 0, 0));
            if(polygon.isInit())
                screenTexture.draw(polygon, context2, {1, 0, 1});
            screenTexture.unbind();
        }

        glm::vec3 RotAxis2;
        float RotAngle2;

        void render(palka::Window& w) override
        {
            static palka::Vec3f ambient = palka::Vec3f{1.0f, 0.5f, 0.31f};
            static palka::Vec3f diffuse = palka::Vec3f{1.0f, 0.5f, 0.31f};
            static palka::Vec3f specular = palka::Vec3f{0.5f, 0.5f, 0.5f};
            static palka::Vec3f l_ambient = palka::Vec3f{0.2f, 0.2f, 0.2f};
            static palka::Vec3f l_diffuse = palka::Vec3f{0.5f, 0.5f, 0.5f};
            static palka::Vec3f l_specular = palka::Vec3f{1.0f, 1.0f, 1.0f};
            renderTex.getCamera().updateCamera(w.getWindow());

            palka::RenderContext context1(&def, &ubo, palka::Mat4f{1.f}, [](palka::ShaderProgram& shader)
            {});

            plane_trasnform = palka::Mat4f{1.f};
            plane_trasnform = glm::rotate(plane_trasnform, RotAngle2, RotAxis2);
            plane_trasnform = glm::scale(plane_trasnform, scale);
            palka::RenderContext context_plane(&def, &ubo, plane_trasnform, [](palka::ShaderProgram& shader)
            {});
            palka::RenderContext context_sphere(&def, &ubo, palka::Mat4f{1.f}, [](palka::ShaderProgram& shader)
            {});
            palka::RenderContext context2(&material_anim, &ubo, palka::Mat4f{1.f}, [](palka::ShaderProgram& shader)
            {
                shader.setUniform("material.ambient", ambient);
                shader.setUniform("material.diffuse", diffuse);
                shader.setUniform("material.specular", specular);
                shader.setUniform("material.shininess", 35.f);

                shader.setUniform("light.ambient", l_ambient);
                shader.setUniform("light.diffuse", l_diffuse);
                shader.setUniform("light.specular", l_specular);
                shader.setUniform("light.position", palka::Vec3f{10, 0, 0});
                shader.setUniform("time", (float) glfwGetTime());
            });

            palka::RenderContext context3(&outline, &ubo, palka::Mat4f{1.f}, [&](palka::ShaderProgram& shader)
            {});

            palka::RenderContext context4(&normals, &ubo, palka::Mat4f{1.f}, [&](palka::ShaderProgram& shader)
            {});

            if(ImGui::CollapsingHeader("Viszulization params"))
            {
                static float scale_value = 1.f;
                ImGui::DragFloat("scale", &scale_value, 0.5f, 1.f, 20.f);
                scale = {scale_value, scale_value, scale_value};
            }
            outlineRenderPass(w);

            renderTex.bind();
            renderTex.clear(palka::Color(0, 0, 0, 0));
            renderTex.drawLine(lx, context1);
            renderTex.drawLine(ly, context1);
            renderTex.drawLine(lz, context1);

            renderTex.draw(sphere, context_sphere, {1, 0, 1});

            if(polygon.isInit())
            {
                //renderTex.draw(polygon, context2, {1, 0, 1});
                renderTex.draw(plane, context_plane, {1, 0, 1});
                renderTex.drawLine(targetPlaneNormal, context1);
                //renderTex.draw(polygon, context4, {1, 0, 1});
                renderTex.draw_t(polygon, context3, {1, 0, 1}, screenTexture.getTexture());
            }
            //renderTex.draw(m, context2, {1, 0, 1});
            renderTex.unbind();

            if(ImGui::Begin("View2"))
            {
                ImGui::SetWindowSize({size.x, size.y});
                ImVec2 pos = ImGui::GetCursorScreenPos();
                ImDrawList* drawList = ImGui::GetWindowDrawList();
                unsigned int f_tex = screenTexture.getTexture().textureID;
                drawList->AddImage((void*) f_tex,
                                   pos,
                                   ImVec2(pos.x + 1280 * 0.85, pos.y + 720 * 0.85),
                                   ImVec2(0, 1),
                                   ImVec2(1, 0));
                ImGui::End();
            }

            ImGui::SetNextWindowPos(ImVec2((Config::WindowSize.x - (size.x)) / 2,
                                           (Config::WindowSize.y - (size.y)) / 2), ImGuiCond_Always, {0, 0});
            if(ImGui::Begin(name.c_str(), &open, win_flag))
            {
                ImGui::SetWindowSize({size.x, size.y});

                if(ImGui::Button("Solve"))
                {
                    auto data = storage->getData<T>();
                    solver3d.init(data, data.columns_count() - 2, data.rows_count() - 1);
                    solver3d.solve();
                    polygon.init(solver3d.points_faces, solver3d.normals);
                    glm::vec3 target_plane = solver3d.target.normal().convert();
                    RotAxis2 = glm::cross(palka::PlaneMesh::normal, target_plane);
                    RotAngle2 = acos(glm::dot(palka::PlaneMesh::normal, target_plane) /
                                     (glm::length(palka::PlaneMesh::normal) * glm::length(target_plane)));
                    plane_trasnform = palka::Mat4f{1.f};
                    plane_trasnform = glm::rotate(plane_trasnform, RotAngle2, RotAxis2);
                    targetPlaneNormal.update({0, 0, 0}, target_plane * 2.f, palka::Color(255, 80, 160));
                    targetPlaneNormal.init();
                }

                if(ImGui::BeginChild("View"))
                {
                    ImGui::SetWindowSize({800, 800});
                    ImVec2 pos = ImGui::GetCursorScreenPos();
                    ImDrawList* drawList = ImGui::GetWindowDrawList();
                    unsigned int f_tex = renderTex.getTexture().textureID;
                    drawList->AddImage((void*) f_tex,
                                       pos,
                                       ImVec2(pos.x + 1280 * 0.85, pos.y + 720 * 0.85),
                                       ImVec2(0, 1),
                                       ImVec2(1, 0));
                    ImGui::EndChild();
                }
                ImGui::End();
            }
        }

        std::pair<int, int> getResul()
        {}

        void update() override
        {}

        void setEvents() override
        {}
    };
}
#endif //MAGPIE_GRAPHICALMETHOD3DVIEW_H

