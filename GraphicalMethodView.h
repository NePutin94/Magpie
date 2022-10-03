#ifndef MAGPIE_GRAPHICALMETHODVIEW_H
#define MAGPIE_GRAPHICALMETHODVIEW_H

#include "UiView.h"
#include "Solver.h"
#include "config.h"
#include "myImGui.h"
#include <imgui_internal.h>
#include <implot.h>
#include <utility>

namespace Magpie
{
    class GraphicalMethodView : public UiView
    {
    private:
        MatrixStorage<double> input;
        GraphicMet2D<double> solver;

        palka::Vec2f result;
        std::vector<double> dataX;
        std::vector<double> dataY;

        bool file_browser_op = false;
        ImGui::FileManager_Context c;
    public:
        GraphicalMethodView(palka::Vec2f pos, palka::Vec2f size, MatrixStorage<double> input, bool open = true, ImGuiWindowFlags w_flag = ImGuiWindowFlags_None)
                : UiView("GraphicalMethodView", pos, size, open, w_flag), input(std::move(input)), c("./", true)
        {}

        void render() override
        {
            ImGui::SetNextWindowPos(ImVec2((Config::WindowSize.x - (size.x)) / 2,
                                           (Config::WindowSize.y - (size.y)) / 2), ImGuiCond_Always, {0, 0});
            ImGui::Begin(name.c_str(), &open, win_flag);
            ImGui::SetWindowSize({size.x, size.y});

            if(ImGui::Button("Solve"))
            {
                solver.init(input, input.columns_count() - 2, input.rows_count() - 1);
                auto res = solver.solve();
                auto u = res.getVisualUnion();
                dataX = u.first;
                dataY = u.second;
                result = res.getResPoint();
            }

            file_browser_op = ImGui::Button("Save");

            c.setOpen(file_browser_op);
            if(auto res = ImGui::FileManager(c); res.first)
            {
                solver.save(c.getPath());
            }
            auto actualSize = ImGui::GetWindowContentRegionMax();
            auto pad = ImGui::GetStyle().WindowPadding;
            if(ImPlot::BeginPlot("My Plot", ImVec2{actualSize.x - pad.x, actualSize.y} * ImVec2{1, 0.7}))
            {
                ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                ImPlot::SetupAxes("x", "y");
                ImPlot::SetupAxesLimits(0, 10, 0, 10);
                ImPlot::PlotLine("My Line Plot", dataX.data(), dataY.data(), dataX.size());
                ImPlot::PlotScatter("Result", &result.x, &result.y, 1);
                ImPlot::EndPlot();
                ImPlot::PopStyleVar();
            }

            ImGui::End();
        }

        std::pair<int, int> getResul()
        {
        }

        void update() override
        {}

        void setEvents() override
        {}
    };
}

#endif //MAGPIE_GRAPHICALMETHODVIEW_H
