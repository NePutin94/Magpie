#ifndef MAGPIE_GRAPHICALMETHODVIEW_H
#define MAGPIE_GRAPHICALMETHODVIEW_H

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

    public:
        GraphicalMethodView(palka::Vec2f pos, palka::Vec2f size, MatrixStorage<T> input, bool open = true,
                            ImGuiWindowFlags w_flag = ImGuiWindowFlags_None)
                : UiView("GraphicalMethodView", pos, size, open, w_flag), c("./", true)
        {
        }

        GraphicalMethodView(std::string name, palka::Vec2f size)
                : UiView(name, size), c("./", true)
        {
        }

        GraphicalMethodView(GraphicalMethodView&& ot) = default;

        void render(palka::Window& w) override
        {
            ImGui::SetNextWindowPos(ImVec2((Config::WindowSize.x - (size.x)) / 2,
                                           (Config::WindowSize.y - (size.y)) / 2), ImGuiCond_Always, {0, 0});
            if(ImGui::Begin(name.c_str(), &open, win_flag))
            {
                ImGui::SetWindowSize({size.x, size.y});

                ImGui::SliderFloat("animation delta", &delta, -10.f, 10.f);
                if(ImGui::Button("Solve"))
                {
                    auto data = storage->getData<T>();
                    solver.init(data, data.columns_count() - 2, data.rows_count() - 1, storage->ptype);
                    //solver.init(input, input.columns_count() - 2, input.rows_count() - 1);
                    auto res = solver.solve();
                    auto u = res.getVisualUnion();
                    Union.X = u.first;
                    Union.Y = u.second;
                    auto res2 = res.getVisualTargetPlot();
                    tagetPlot.X = res2.first;
                    tagetPlot.Y = res2.second;
                    tagetPlotAnim = tagetPlot;
                    auto t_vec = -res.plot_perpendicular.getGrad().convert();
                    vec = {(double)t_vec.x,(double)t_vec.y};
                    GradVec.X = {0, vec.x};
                    GradVec.Y = {0, vec.y};
                    result = res.getResPoint();
                    solveDone = true;
                    resultValue = res.getResultValue();
                    auto szX = sqrt(vec.x * vec.x + vec.y * vec.y);
                    for(int i = 0; i < tagetPlot.X.size(); ++i)
                    {
                        tagetPlot.X[i] = tagetPlot.X[i] + 10.f * -vec.x / szX;
                        tagetPlot.Y[i] = tagetPlot.Y[i] + 10.f * -vec.y / szX;
                    }

                    GradVec.X[0] = GradVec.X[0] + 10.f * -vec.x / szX;
                    GradVec.Y[0] = GradVec.Y[0] + 10.f * -vec.y / szX;

                    GradVec.X[1] = GradVec.X[1] + 10.f * -vec.x / szX;
                    GradVec.Y[1] = GradVec.Y[1] + 10.f * -vec.y / szX;

                    GradVecAnim = GradVec;
                    tagetPlotAnim = tagetPlot;
                }

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
                        if(ImPlot::BeginPlot("My Plot", ImVec2{actualSize.x - pad.x - 40.f, actualSize.y} * ImVec2{1, 0.6}, ImPlotFlags_Equal))
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

                ImGui::End();
            }
        }

        bool check(palka::Vec2<double> p)
        {
            return compare_float<double>((p.x - tagetPlotAnim.X[0]) / (tagetPlotAnim.X[1] - tagetPlotAnim.X[0]),
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
