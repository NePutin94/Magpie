#ifndef MAGPIE_GRAPHICALMETHODVIEW_H
#define MAGPIE_GRAPHICALMETHODVIEW_H

#include "UiView.h"
#include "Solver.h"
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

namespace Magpie
{
    template<class T>
    class GraphicalMethodView : public UiView
    {
    private:
        template<class T2>
        struct MLine
        {
            std::vector<T2> X;
            std::vector<T2> Y;
        };
        bool solveDone = false;
        //MatrixStorage<T> input;
        GraphicMet2D<T> solver;
        GraphicMet3D<T> solver3d;

        palka::Vec2<double> result;
        MLine<double> Union;

        MLine<double> tagetPlot;

        MLine<double> tagetPlotAnim;

        MLine<double> GradVec;

        MLine<double> GradVecAnim;
        float delta = 0;
        palka::Vec2<double> vec;
        bool file_browser_op = false;
        double resultValue;
        ImGui::FileManager_Context c;

        //3D staff//
        palka::RenderTexture renderTex;
        palka::RenderTexture screenTexture;
        palka::ShaderProgram material_light;
        palka::ShaderProgram normals;
        palka::ShaderProgram def;
        palka::ShaderProgram outline;
        palka::UniformBuffer ubo;
        palka::Renderer::Line lx{{-10, 0, 0},
                                 {10, 0, 0},
                                 palka::Color(255, 0, 0)};
        palka::Renderer::Line ly{{0, -10, 0},
                                 {0, 10, 0},
                                 palka::Color(0, 255, 0)};
        palka::Renderer::Line lz{{0, 0, -10},
                                 {0, 0, 10},
                                 palka::Color(0, 0, 255)};
        palka::PolygonMesh polygon;

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
            material_light.createProgram();
            material_light.addShader(l3);
            material_light.addShader(l4);
            material_light.addShader(l5);
            material_light.linkProgram();
            material_light.UBOBindingTo(material_light.getUBOIndex("matrixBuffer"), 0);

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
        }

    public:
        GraphicalMethodView(palka::Vec2f pos, palka::Vec2f size, MatrixStorage<T> input, bool open = true,
                            ImGuiWindowFlags w_flag = ImGuiWindowFlags_None)
                : UiView("GraphicalMethodView", pos, size, open, w_flag), c("./", true), renderTex({1920.f, 1080.f}),
                  screenTexture({1920.f, 1080.f})
        {
            init3D();
        }

        GraphicalMethodView(std::string name, palka::Vec2f size)
                : UiView(name, size), c("./", true),
                  renderTex({1920.f, 1080.f}),
                  screenTexture({1920.f, 1080.f})
        {
            init3D();
        }

        GraphicalMethodView(GraphicalMethodView&& ot) = default;

        void renderScreen(palka::Window& w)
        {
            static palka::Vec3f ambient = palka::Vec3f{1.0f, 0.5f, 0.31f};
            static palka::Vec3f diffuse = palka::Vec3f{1.0f, 0.5f, 0.31f};
            static palka::Vec3f specular = palka::Vec3f{0.5f, 0.5f, 0.5f};
            static palka::Vec3f l_ambient = palka::Vec3f{0.2f, 0.2f, 0.2f};
            static palka::Vec3f l_diffuse = palka::Vec3f{0.5f, 0.5f, 0.5f};
            static palka::Vec3f l_specular = palka::Vec3f{1.0f, 1.0f, 1.0f};
            screenTexture.getCamera().updateCamera(w.getWindow());

            palka::RenderContext context2(&material_light, &ubo, palka::Mat4f{1.f}, [](palka::ShaderProgram& shader)
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

        void render(palka::Window& w) override
        {
            static palka::Vec3f ambient = palka::Vec3f{1.0f, 0.5f, 0.31f};
            static palka::Vec3f diffuse = palka::Vec3f{1.0f, 0.5f, 0.31f};
            static palka::Vec3f specular = palka::Vec3f{0.5f, 0.5f, 0.5f};
            static palka::Vec3f l_ambient = palka::Vec3f{0.2f, 0.2f, 0.2f};
            static palka::Vec3f l_diffuse = palka::Vec3f{0.5f, 0.5f, 0.5f};
            static palka::Vec3f l_specular = palka::Vec3f{1.0f, 1.0f, 1.0f};
            //renderScreen(w);
            renderTex.getCamera().updateCamera(w.getWindow());

            palka::RenderContext context1(&def, &ubo, palka::Mat4f{1.f}, [](palka::ShaderProgram& shader)
            {});
            palka::RenderContext context2(&material_light, &ubo, palka::Mat4f{1.f}, [](palka::ShaderProgram& shader)
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

            renderTex.bind();
            renderTex.clear(palka::Color(0, 0, 0, 0));
            renderTex.drawLine(lx, context1);
            renderTex.drawLine(ly, context1);
            renderTex.drawLine(lz, context1);
            if(polygon.isInit())
            {
                renderTex.draw(polygon, context2, {1, 0, 1});
                //renderTex.draw(polygon, context4, {1, 0, 1});
                // renderTex.draw_t(polygon, context3, {1, 0, 1}, screenTexture.getTexture());
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

                ImGui::SliderFloat("animation delta", &delta, -10.f, 10.f);
                if(ImGui::Button("Solve3D"))
                {
                    auto data = storage->getData<T>();
                    solver3d.init(data, data.columns_count() - 2, data.rows_count() - 1);
                    // solver3d.init(input, input.columns_count() - 2, input.rows_count() - 1);
                    solver3d.solve();
                    polygon.init(solver3d.points_faces, solver3d.normals);
                }
//                if(ImGui::Button("Solve"))
//                {
//                    auto data = storage->getData<T>();
//                    solver.init(data, data.columns_count() - 2, data.rows_count() - 1);
//                    //solver.init(input, input.columns_count() - 2, input.rows_count() - 1);
//                    auto res = solver.solve();
//                    auto u = res.getVisualUnion();
//                    Union.X = u.first;
//                    Union.Y = u.second;
//                    auto res2 = res.getVisualTargetPlot();
//                    tagetPlot.X = res2.first;
//                    tagetPlot.Y = res2.second;
//                    tagetPlotAnim = tagetPlot;
//                    vec = -res.plot_perpendicular.getGrad().convert();
//                    GradVec.X = {0, vec.x};
//                    GradVec.Y = {0, vec.y};
//                    result = res.getResPoint().convert();
//                    solveDone = true;
//                    resultValue = res.getResultValue();
//                    auto szX = sqrt(vec.x * vec.x + vec.y * vec.y);
//                    for(int i = 0; i < tagetPlot.X.size(); ++i)
//                    {
//                        tagetPlot.X[i] = tagetPlot.X[i] + 10.f * -vec.x / szX;
//                        tagetPlot.Y[i] = tagetPlot.Y[i] + 10.f * -vec.y / szX;
//                    }
//
//                    GradVec.X[0] = GradVec.X[0] + 10.f * -vec.x / szX;
//                    GradVec.Y[0] = GradVec.Y[0] + 10.f * -vec.y / szX;
//
//                    GradVec.X[1] = GradVec.X[1] + 10.f * -vec.x / szX;
//                    GradVec.Y[1] = GradVec.Y[1] + 10.f * -vec.y / szX;
//
//                    GradVecAnim = GradVec;
//                    tagetPlotAnim = tagetPlot;
//                }

                file_browser_op = ImGui::Button("Save");

                if(solveDone)
                {
                    c.setOpen(file_browser_op);
                    if(auto res = ImGui::FileManager(c); res.first)
                    {
                        solver.save(c.getPath());
                    }
                    auto actualSize = ImGui::GetWindowContentRegionMax();
                    auto pad = ImGui::GetStyle().WindowPadding;
                    if(ImGui::BeginChild("Plot", ImVec2{actualSize.x - pad.x - 40.f, actualSize.y} * ImVec2{1, 0.6}))
                    {
                        if(ImPlot::BeginPlot("My Plot", ImVec2{0, 0}, ImPlotFlags_Equal))
                        {
                            ImPlot::SetupAxis(ImAxis_X2, "X", ImPlotAxisFlags_AuxDefault);
                            ImPlot::SetupAxis(ImAxis_Y2, "Y", ImPlotAxisFlags_AuxDefault);
                            ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                            ImPlot::PlotLine("My Line Plot", Union.X.data(), Union.Y.data(), Union.X.size());
                            ImPlot::PlotScatter("Result", &result.x, &result.y, 1);
                            ImPlot::PlotLine("Target Function", tagetPlotAnim.X.data(), tagetPlotAnim.Y.data(), tagetPlotAnim.X.size());
                            ImPlot::PlotLine("Perp Function", GradVecAnim.X.data(), GradVecAnim.Y.data(), GradVecAnim.X.size());
                            ImPlot::SetAxes(ImAxis_X2, ImAxis_Y2);
                            ImPlot::EndPlot();
                            ImPlot::PopStyleVar();
                        }
                        ImGui::EndChild();
                    }
                    if(ImGui::CollapsingHeader("Solve results"))
                    {
                        ImGui::Text("Union points: ");
                        for(int i = 0; i < Union.X.size(); ++i)
                        {
                            ImGui::Text("point %i : {%f,%f}", i, Union.X[i], Union.Y[i]);
                        }
                        ImGui::Text("F gradient: {%f, %f}", vec.x, vec.y);
                        ImGui::Text("X* result: {%f, %f}", result.x, result.y);
                        ImGui::Text("F* result: %f", resultValue);
                    }
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

        bool check(palka::Vec2<double> p)
        {
            return compare_float<double>((p.x - tagetPlotAnim.X[0]) / (tagetPlotAnim.X[1] - tagetPlotAnim.X[0]),
                                    (p.y - tagetPlotAnim.Y[0]) / (tagetPlotAnim.Y[1] - tagetPlotAnim.Y[0]), 0.0001);
        }

        void update() override
        {
            palka::debug(renderTex.getCamera(), "camera");
            static bool back = false;
            if(solveDone)
            {
                if(!check(result))
                {
                    delta += ImGui::GetIO().DeltaTime * 2;
                    auto szX = std::sqrt(vec.x * vec.x + vec.y * vec.y);
                    for(int i = 0; i < tagetPlot.X.size(); ++i)
                    {
                        tagetPlotAnim.X[i] = tagetPlot.X[i] + vec.x * delta / szX;
                        tagetPlotAnim.Y[i] = tagetPlot.Y[i] + vec.y * delta / szX;
                    }

                    GradVecAnim.X[0] = GradVec.X[0] + vec.x * delta / szX;
                    GradVecAnim.Y[0] = GradVec.Y[0] + vec.y * delta / szX;

                    GradVecAnim.X[1] = GradVec.X[1] + vec.x * delta / szX;
                    GradVecAnim.Y[1] = GradVec.Y[1] + vec.y * delta / szX;
                }
            }
        }

        void setEvents() override
        {}
    };
}

#endif //MAGPIE_GRAPHICALMETHODVIEW_H
