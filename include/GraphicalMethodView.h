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

namespace Magpie
{
    class GraphicalMethodView : public UiView
    {
    private:
        struct MLine
        {
            std::vector<double> X;
            std::vector<double> Y;
        };
        bool solveDone = false;
        MatrixStorage<Fractus> input;
        GraphicMet2D<Fractus> solver;

        palka::Vec2f result;
        MLine Union;

        MLine tagetPlot;

        MLine tagetPlotAnim;

        MLine GradVec;

        MLine GradVecAnim;
        float delta = 0;
        palka::Vec2f vec;
        bool file_browser_op = false;
        double resultValue;
        ImGui::FileManager_Context c;

        //3D staff//
        palka::RenderTexture renderTex;
        palka::ShaderProgram material_light;
        palka::ShaderProgram def;
        palka::UniformBuffer ubo;
        // std::vector<palka::PlaneMesh> planes;
        palka::Renderer::Line lx{{-10, 0, 0},
                                 {10, 0, 0},
                                 palka::Color(255, 0, 0)};
        palka::Renderer::Line ly{{0, -10, 0},
                                 {0, 10, 0},
                                 palka::Color(0, 255, 0)};
        palka::Renderer::Line lz{{0, 0, -10},
                                 {0, 0, 10},
                                 palka::Color(0, 0, 255)};
        // palka::Sprite renderSp;
        palka::StaticMesh m;
        void init3D()
        {
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
            renderTex.getViewport().setCenter({500 / 2.f, 500 / 2.f});
            //  renderSp.setTexture(renderTex.getTexture());
            //  renderSp.setPosition({500, 0});
        }

    public:
        GraphicalMethodView(palka::Vec2f pos, palka::Vec2f size, MatrixStorage<Fractus> input, bool open = true,
                            ImGuiWindowFlags w_flag = ImGuiWindowFlags_None)
                : UiView("GraphicalMethodView", pos, size, open, w_flag), input(std::move(input)), c("./", true), renderTex({500, 500})
        {
            init3D();
        }

        GraphicalMethodView(GraphicalMethodView&& ot) = default;

        void render(palka::Window& w) override
        {

            palka::RenderContext context1(&def, &ubo, palka::Mat4f{1.f}, [](palka::ShaderProgram& shader)
            {});
            renderTex.bind();
            renderTex.clear(palka::Color(0, 0, 0, 0));
            renderTex.draw(m, context1, {1,0,1});
            renderTex.unbind();

            ImGui::Begin("Game Window");
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            unsigned int f_tex = renderTex.getTexture().textureID;
            drawList->AddImage((void*) f_tex,
                               pos,
                               ImVec2(pos.x + 512, pos.y + 512),
                               ImVec2(0, 1),
                               ImVec2(1, 0));
            ImGui::End();

//            ImGui::SetNextWindowPos(ImVec2((Config::WindowSize.x - (size.x)) / 2,
//                                           (Config::WindowSize.y - (size.y)) / 2), ImGuiCond_Always, {0, 0});
//            if(ImGui::Begin(name.c_str(), &open, win_flag))
//            {
//                ImGui::SetWindowSize({size.x, size.y});
//
//                ImGui::SliderFloat("animation delta", &delta, -10.f, 10.f);
//
//                if(ImGui::Button("Solve"))
//                {
//                    solver.init(input, input.columns_count() - 2, input.rows_count() - 1);
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
//
//                file_browser_op = ImGui::Button("Save");
//
//                if(solveDone)
//                {
//                    c.setOpen(file_browser_op);
//                    if(auto res = ImGui::FileManager(c); res.first)
//                    {
//                        solver.save(c.getPath());
//                    }
//                    auto actualSize = ImGui::GetWindowContentRegionMax();
//                    auto pad = ImGui::GetStyle().WindowPadding;
//                    if(ImGui::BeginChild("Plot", ImVec2{actualSize.x - pad.x - 40.f, actualSize.y} * ImVec2{1, 0.6}))
//                    {
//                        if(ImPlot::BeginPlot("My Plot", ImVec2{0, 0}, ImPlotFlags_Equal))
//                        {
//                            ImPlot::SetupAxis(ImAxis_X2, "X", ImPlotAxisFlags_AuxDefault);
//                            ImPlot::SetupAxis(ImAxis_Y2, "Y", ImPlotAxisFlags_AuxDefault);
//                            ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
//                            ImPlot::PlotLine("My Line Plot", Union.X.data(), Union.Y.data(), Union.X.size());
//                            ImPlot::PlotScatter("Result", &result.x, &result.y, 1);
//                            ImPlot::PlotLine("Target Function", tagetPlotAnim.X.data(), tagetPlotAnim.Y.data(), tagetPlotAnim.X.size());
//                            ImPlot::PlotLine("Perp Function", GradVecAnim.X.data(), GradVecAnim.Y.data(), GradVecAnim.X.size());
//                            ImPlot::SetAxes(ImAxis_X2, ImAxis_Y2);
//                            ImPlot::EndPlot();
//                            ImPlot::PopStyleVar();
//                        }
//                        ImGui::EndChild();
//                    }
//                    if(ImGui::CollapsingHeader("Solve results"))
//                    {
//                        ImGui::Text("Union points: ");
//                        for(int i = 0; i < Union.X.size(); ++i)
//                        {
//                            ImGui::Text("point %i : {%f,%f}", i, Union.X[i], Union.Y[i]);
//                        }
//                        ImGui::Text("F gradient: {%f, %f}", vec.x, vec.y);
//                        ImGui::Text("X* result: {%f, %f}", result.x, result.y);
//                        ImGui::Text("F* result: %f", resultValue);
//                    }
//                }
//                ImGui::End();
//            }
        }

        std::pair<int, int> getResul()
        {}

        bool check(palka::Vec2f p)
        {
            return compare_float((p.x - tagetPlotAnim.X[0]) / (tagetPlotAnim.X[1] - tagetPlotAnim.X[0]),
                                 (p.y - tagetPlotAnim.Y[0]) / (tagetPlotAnim.Y[1] - tagetPlotAnim.Y[0]), 0.0001);
        }

        void update() override
        {
            static bool back = false;
            if(solveDone)
            {
                if(!check(result))
                {
                    delta += ImGui::GetIO().DeltaTime * 2;
                    auto szX = sqrt(vec.x * vec.x + vec.y * vec.y);
                    for(int i = 0; i < tagetPlot.X.size(); ++i)
                    {
                        tagetPlotAnim.X[i] = tagetPlot.X[i] + delta * vec.x / szX;
                        tagetPlotAnim.Y[i] = tagetPlot.Y[i] + delta * vec.y / szX;
                    }

                    GradVecAnim.X[0] = GradVec.X[0] + delta * vec.x / szX;
                    GradVecAnim.Y[0] = GradVec.Y[0] + delta * vec.y / szX;

                    GradVecAnim.X[1] = GradVec.X[1] + delta * vec.x / szX;
                    GradVecAnim.Y[1] = GradVec.Y[1] + delta * vec.y / szX;
                }
            }
        }

        void setEvents() override
        {}
    };
}

#endif //MAGPIE_GRAPHICALMETHODVIEW_H
