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
#include <cmath>

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
        T result_value;
        glm::vec3 result_point;
        palka::Vec2f scale;
        //3D staff//
        palka::RenderTexture renderTex;
        palka::RenderTexture screenTexture;
        palka::ShaderProgram material_anim;
        palka::ShaderProgram outline_anim;
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
        palka::Vec3f ambient = palka::Vec3f{1.0f, 0.5f, 0.31f};
        palka::Vec3f diffuse = palka::Vec3f{1.0f, 0.5f, 0.31f};
        palka::Vec3f specular = palka::Vec3f{0.5f, 0.5f, 0.5f};
        palka::Vec3f l_ambient = palka::Vec3f{0.2f, 0.2f, 0.2f};
        palka::Vec3f l_diffuse = palka::Vec3f{0.5f, 0.5f, 0.5f};
        palka::Vec3f l_specular = palka::Vec3f{1.0f, 1.0f, 1.0f};
        //target plane trasnform
        palka::Mat4f plane_trasnform{1.f};
        palka::Mat4f normal_trasnform{1.f};
        palka::Mat4f sphere_trasnform{1.f};
        palka::Vec3f plane_scale{1.f};
        glm::vec3 sphere_pos{0.f};
        glm::vec3 plane_pos{0.f};
        glm::vec3 target_plane;
        float offset = 0;

        bool animation = false;
        float time;

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

            palka::_Shader a3("Data\\Shaders\\outline.frag", palka::_Shader::FRAGMENT);
            palka::_Shader a4("Data\\Shaders\\explode.vert", palka::_Shader::VERTEX);
            palka::_Shader a5("Data\\Shaders\\explode.gs", palka::_Shader::GEOMETRY);
            outline_anim.createProgram();
            outline_anim.addShader(a3);
            outline_anim.addShader(a4);
            outline_anim.addShader(a5);
            outline_anim.linkProgram();
            outline_anim.UBOBindingTo(outline_anim.getUBOIndex("matrixBuffer"), 0);

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
        void navLayout(bool isReady)
        {
            constexpr int b_padding = 40.f;
            ImVec2 b_size{80.f, 35.f};
            ImGui::Spacing();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x * .5f);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - b_size.x / 2.f);
            if(ImGui::Button("Back", b_size))
            {
                nextSatet = States::Menu;
                clearState = true;
                sceneCallback();
            }
        }
    public:
        GraphicalMethod3DView(std::string name, palka::Vec2f scale)
                : UiView(name, Config::WindowSize * scale), c("./", true),
                  renderTex({1920.f, 1080.f}),
                  screenTexture({1920.f, 1080.f}),
                  sphere(0.1, 20, 20),
                  scale(scale)
        {
            init3D();
        }

        GraphicalMethod3DView(GraphicalMethod3DView&& ot) = default;

        void outlineRenderPass(palka::Window& w)
        {
            palka::RenderContext context2(&material_anim, &ubo, palka::Mat4f{1.f}, [this](palka::ShaderProgram& shader)
            {
                shader.setUniform("material.ambient", ambient);
                shader.setUniform("material.diffuse", diffuse);
                shader.setUniform("material.specular", specular);
                shader.setUniform("material.shininess", 35.f);

                shader.setUniform("light.ambient", l_ambient);
                shader.setUniform("light.diffuse", l_diffuse);
                shader.setUniform("light.specular", l_specular);
                shader.setUniform("light.position", palka::Vec3f{10, 0, 0});

                shader.setUniform("time", time);
            });
            screenTexture.bind();
            screenTexture.clear(palka::Color(0, 0, 0, 0));
            if(polygon.isInit())
                screenTexture.draw(polygon, context2, {1, 0, 1});
            screenTexture.unbind();
        }

        glm::vec3 RotAxis2;
        float RotAngle2;
        bool AnyHovered;

        void render(palka::Window& w) override
        {
            if(AnyHovered)
            {
                renderTex.getCamera().updateCamera(w.getWindow());
                screenTexture.getCamera().updateCamera(w.getWindow());
            }

            if(animation)
                time = (float) glfwGetTime();
            else
                time = 3.0 * M_PI / 2.0;

            plane_trasnform = palka::Mat4f{1.f};
            plane_trasnform = glm::translate(plane_trasnform, plane_pos);
            plane_trasnform = glm::scale(plane_trasnform, plane_scale);
            plane_trasnform = glm::rotate(plane_trasnform, RotAngle2, RotAxis2);

            sphere_trasnform = palka::Mat4f{1.f};
            sphere_trasnform = glm::translate(sphere_trasnform, sphere_pos);

            normal_trasnform = palka::Mat4f{1.f};
            normal_trasnform = glm::translate(normal_trasnform, plane_pos);

            palka::RenderContext context_line(&def, &ubo, palka::Mat4f{1.f}, [](palka::ShaderProgram& shader)
            {});
            palka::RenderContext context_normal(&def, &ubo, normal_trasnform, [](palka::ShaderProgram& shader)
            {});
            palka::RenderContext context_plane(&def, &ubo, plane_trasnform, [](palka::ShaderProgram& shader)
            {});
            palka::RenderContext context_sphere(&def, &ubo, sphere_trasnform, [](palka::ShaderProgram& shader)
            {});
            palka::RenderContext context3(&outline_anim, &ubo, palka::Mat4f{1.f}, [&](palka::ShaderProgram& shader)
            {
                shader.setUniform("time", time);
            });

            outlineRenderPass(w);

            renderTex.bind();
            renderTex.clear(palka::Color(0, 0, 0, 0));
            renderTex.drawLine(lx, context_line);
            renderTex.drawLine(ly, context_line);
            renderTex.drawLine(lz, context_line);
            renderTex.draw(sphere, context_sphere, {1, 0, 1});
            if(polygon.isInit())
            {
                renderTex.draw(plane, context_plane, {1, 0, 1});
                renderTex.drawLine(targetPlaneNormal, context_normal);
                renderTex.draw_t(polygon, context3, {1, 0, 1}, screenTexture.getTexture());
            }
            renderTex.unbind();
        }

        std::pair<int, int> getResul()
        {}

        void update() override
        {
            size = Config::WindowSize * scale;
            ImGui::SetNextWindowPos(ImVec2((Config::WindowSize.x - (size.x)) / 2,
                                           (Config::WindowSize.y - (size.y)) / 2), ImGuiCond_Always, {0, 0});
            if(ImGui::Begin(name.c_str(), &open, win_flag))
            {
                ImGui::SetWindowSize(ImVec2{size.x, size.y});
                if(ImGui::CollapsingHeader("Visualization params"))
                {
                    static float scale_value = 1.f;
                    ImGui::DragFloat("plane scale", &scale_value, 0.5f, 1.f, 20.f);
                    plane_scale = {scale_value, scale_value, scale_value};
                    ImGui::DragFloat("plane offset", &offset, 0.5f);
                    plane_pos = -target_plane * glm::vec3{offset, offset, offset};
                    ImGui::Checkbox("animation", &animation);
                }
                if(ImGui::Button("Solve"))
                {
                    auto data = storage->getData<T>();
                    solver3d.init(data, data.columns_count() - 2, data.rows_count() - 1, storage->ptype);
                    auto result = solver3d.solve();
                    result_value = result.result_value;
                    result_point = result.result_point;
                    sphere_pos = result_point;
                    target_plane = (storage->ptype == DataStorage::Minimization) ?
                                   -solver3d.target.getGrad().convert() :
                                   solver3d.target.getGrad().convert();
                    offset = glm::distance({0, 0, 0}, result_point) / 2;
                    plane_pos = -target_plane * glm::vec3{offset, offset, offset};
                    polygon.init(solver3d.points_faces, solver3d.normals);
                    RotAxis2 = glm::cross(palka::PlaneMesh::normal, target_plane);
                    RotAngle2 = acos(glm::dot(palka::PlaneMesh::normal, target_plane) /
                                     (glm::length(palka::PlaneMesh::normal) * glm::length(target_plane)));
                    plane_trasnform = palka::Mat4f{1.f};
                    plane_trasnform = glm::rotate(plane_trasnform, RotAngle2, RotAxis2);
                    targetPlaneNormal.update({0, 0, 0}, target_plane * 2.f, palka::Color(255, 80, 160));
                    targetPlaneNormal.init();
                }

                if(ImGui::CollapsingHeader("Solve results"))
                {
                    ImGui::Text("X* result: {%f, %f, %f}", result_point.x, result_point.y, result_point.z);
                    ImGui::Text("F* result: %f", (double) result_value);
                }
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40.f);
                AnyHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_ChildWindows);
                if(ImGui::BeginChild("View"))
                {
                    ImVec2 pos = ImGui::GetCursorScreenPos();
                    auto maxSx = ImGui::GetContentRegionMax();
                    auto aspectSize = calcSize(maxSx);
                    auto pos2 = ImVec2(pos.x + aspectSize.x, pos.y + aspectSize.y);
                    auto rect = maxSx - (pos2 - pos);
                    ImVec2 off = rect / 2;
                    ImDrawList* drawList = ImGui::GetWindowDrawList();
                    unsigned int f_tex = renderTex.getTexture().textureID;
                    drawList->AddImage((void*) f_tex,
                                       pos + off,
                                       pos2 + off,
                                       ImVec2(0, 1),
                                       ImVec2(1, 0));
                    ImGui::EndChild();
                }
                navLayout(true);
                ImGui::End();
            }
        }

        ImVec2 calcSize(ImVec2 initSize)
        {
            auto w = initSize.x / 16;
            auto h = initSize.y / 9;
            if(h * 16 <= initSize.x)
                return {h * 16 + 5, initSize.y};
            return {initSize.x, w * 9 + 5};

        }

        void setEvents() override
        {}
    };
}
#endif //MAGPIE_GRAPHICALMETHOD3DVIEW_H

