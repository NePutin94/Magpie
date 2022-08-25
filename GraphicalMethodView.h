#ifndef MAGPIE_GRAPHICALMETHODVIEW_H
#define MAGPIE_GRAPHICALMETHODVIEW_H

#include "UiView.h"
#include "Solver.h"
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
    public:
        GraphicalMethodView(palka::Vec2f pos, palka::Vec2f size, MatrixStorage<double> input, bool open = true, ImGuiWindowFlags w_flag = ImGuiWindowFlags_None)
                : UiView("GraphicalMethodView", pos, size, open, w_flag), input(std::move(input))
        {}

        void render() override
        {
            ImGui::Begin(name.c_str(), &open, win_flag);

            if(ImGui::Button("Solve"))
            {
                solver.init(input, input.columns_count() - 2, input.rows_count() - 1);
                auto res = solver.solve();
                auto u = res.getVisualUnion();
                dataX = u.first;
                dataY = u.second;
                result = res.getResPoint();
            }

            if(ImPlot::BeginPlot("My Plot"))
            {
                ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
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
