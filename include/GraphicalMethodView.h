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
        bool hasSolution = false;
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
        ImGui::FileManager_Context c;
        double resultValue;
        float offset = 0;
        float maxDelta;
        bool animation = true;
        std::vector<double> unionPointsx;
        std::vector<double> unionPointsy;
        palka::Vec2f scale;
    public:
        GraphicalMethodView(std::string name, palka::Vec2f scale)
                : UiView(name, Config::WindowSize * scale), c("./", true), scale(scale)
        {
        }

        GraphicalMethodView(GraphicalMethodView&& ot) = default;

        void render(palka::Window& w) override
        {
            size = Config::WindowSize * scale;
            ImGui::SetNextWindowPos(ImVec2((Config::WindowSize.x - (size.x)) / 2,
                                           (Config::WindowSize.y - (size.y)) / 2), ImGuiCond_Always, {0, 0});
            if(ImGui::Begin(name.c_str(), &open, win_flag))
            {
                ImGui::SetWindowSize(ImVec2{size.x, size.y});

                if(ImGui::SliderFloat("delta", &delta, 0, maxDelta))
                    animation = false;
                if(ImGui::Button("Solve"))
                {
                    auto data = storage->getData<T>();
                    solver.init(data, data.columns_count() - 2, data.rows_count() - 1, storage->ptype);
                    auto res = solver.solve();
                    auto u = res.getVisualUnion();
                    Union.X = u.first;
                    Union.Y = u.second;
                    auto res2 = res.getVisualTargetPlot();
                    tagetPlot.X = res2.first;
                    tagetPlot.Y = res2.second;
                    tagetPlotAnim = tagetPlot;
                    auto t_vec = (storage->ptype == DataStorage::Minimization) ?
                                 -res.plot_perpendicular.getGrad().convert() :
                                 res.plot_perpendicular.getGrad().convert();
                    vec = {(double) t_vec.x, (double) t_vec.y};
                    GradVec.X = {0, vec.x};
                    GradVec.Y = {0, vec.y};
                    result = res.getResPoint();
                    auto start_point = result / palka::Vec2<double>{2.f, 2.f};
                    double l = glm::distance({0.f, 0.f}, start_point.convert());
                    if(l <= 1.f)
                        l = 7.f;
                    solveDone = true;
                    resultValue = res.getResultValue();
                    auto szX = sqrt(vec.x * vec.x + vec.y * vec.y);
                    for(auto& p: res.getUnion())
                    {
                        if(p == result)
                            continue;
                        unionPointsx.emplace_back(p.x);
                        unionPointsy.emplace_back(p.y);
                    }
                    if(res.getType() != GraphicsResult<T>::Type::openInfinity)
                    {
                        for(int i = 0; i < tagetPlot.X.size(); ++i)
                        {
                            tagetPlot.X[i] = tagetPlot.X[i] + l * -vec.x / szX;
                            tagetPlot.Y[i] = tagetPlot.Y[i] + l * -vec.y / szX;
                        }

                        GradVec.X[0] = GradVec.X[0] + l * -vec.x / szX;
                        GradVec.Y[0] = GradVec.Y[0] + l * -vec.y / szX;

                        GradVec.X[1] = GradVec.X[1] + l * -vec.x / szX;
                        GradVec.Y[1] = GradVec.Y[1] + l * -vec.y / szX;

                        offset = l / 5;
                        maxDelta = 3 * l;
                        GradVecAnim = GradVec;
                        tagetPlotAnim = tagetPlot;
                        hasSolution = true;
                    }
                }

                if(solveDone)
                {
                    auto actualSize = ImGui::GetWindowContentRegionMax();
                    auto pad = ImGui::GetStyle().WindowPadding;
                    if(ImGui::BeginChild("Plot", ImVec2{actualSize.x - pad.x - 40.f, actualSize.y} * ImVec2{1, 0.6}))
                    {
                        if(ImPlot::BeginPlot("Result", ImVec2{actualSize.x - pad.x - 40.f, actualSize.y} * ImVec2{1, 0.6}, ImPlotFlags_Equal))
                        {
                            ImPlot::SetupAxis(ImAxis_X2, "X", ImPlotAxisFlags_AuxDefault);
                            ImPlot::SetupAxis(ImAxis_Y2, "Y", ImPlotAxisFlags_AuxDefault);
                            ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                            ImPlot::PlotScatter("Union Points", unionPointsx.data(), unionPointsy.data(), unionPointsx.size());
                            ImPlot::PlotScatter("Result", &result.x, &result.y, 1);
                            ImPlot::PlotLine("Union", Union.X.data(), Union.Y.data(), Union.X.size());
                            ImPlot::PlotLine("Target Function", tagetPlotAnim.X.data(), tagetPlotAnim.Y.data(), tagetPlotAnim.X.size());
                            ImPlot::PlotLine("Gradient", GradVecAnim.X.data(), GradVecAnim.Y.data(), GradVecAnim.X.size());
                            ImPlot::SetAxes(ImAxis_X2, ImAxis_Y2);
                            ImPlot::EndPlot();
                            ImPlot::PopStyleVar();
                        }
                        ImGui::EndChild();
                    }
                    if(ImGui::CollapsingHeader("Solve results"))
                    {
                        if(hasSolution)
                        {
                            ImGui::Text("Union points: ");
                            for(int i = 0; i < Union.X.size(); ++i)
                                ImGui::Text("Point %i : {%f,%f}", i, Union.X[i], Union.Y[i]);
                            ImGui::Text("F gradient: {%f, %f}", vec.x, vec.y);
                            ImGui::Text("X* result: {%f, %f}", result.x, result.y);
                            ImGui::Text("F* result: %f", resultValue);
                        } else
                        {
                            ImGui::Text("Union points: ");
                            for(int i = 0; i < Union.X.size(); ++i)
                                ImGui::Text("Point %i : {%f,%f}", i, Union.X[i], Union.Y[i]);
                            ImGui::Text("There is no solution to the problem.");
                        }
                    }
                }
                navLayout(true);
                ImGui::End();
            }
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

        bool check(palka::Vec2<double> p)
        {
            return compare_float<double>((p.x - tagetPlotAnim.X[0]) / (tagetPlotAnim.X[1] - tagetPlotAnim.X[0]),
                                         (p.y - tagetPlotAnim.Y[0]) / (tagetPlotAnim.Y[1] - tagetPlotAnim.Y[0]), 0.0001);
        }

        void update() override
        {
            static bool back = false;
            if(solveDone && hasSolution)
            {
                if(!check(result) || !animation)
                {
                    animation = true;
                    delta += ImGui::GetIO().DeltaTime * offset;
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
