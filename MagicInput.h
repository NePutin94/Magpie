//
// Created by dimka on 9/4/2022.
//

#ifndef MAGPIE_MAGICINPUT_H
#define MAGPIE_MAGICINPUT_H

#include "UiView.h"
#include <imgui_internal.h>
#include <implot.h>
#include <fmt/core.h>
#include "Solver.h"

namespace Magpie
{
    class MagicInput : public UiView
    {
        float roundoff(double value, unsigned char prec)
        {
            float pow_10 = pow(10.0f, (double) prec);
            return round(value * pow_10) / pow_10;
        }

        void layout2(const char* label, double& val, int col)
        {
            auto label_sz = ImGui::CalcTextSize("value");
            ImGui::AlignTextToFramePadding();
            constexpr float input_w = 50;
            constexpr float padding = 2;
            const float full_w = input_w + label_sz.x + padding;
            float ww = ImGui::GetColumnWidth(col);
            const float center_offset = (ww - full_w) / 2.f;
            auto start_pos = ImGui::GetCursorPos();
            auto start_pos_l = ImGui::GetCursorScreenPos();
            ImGui::SetCursorPosX(start_pos.x + center_offset);
            ImGui::SetCursorPosY(start_pos.y + 60 / 2.f - label_sz.y);
            ImGui::TextUnformatted("value");
            ImGui::SameLine();

            ImGui::SetCursorPos(start_pos + ImVec2(center_offset + label_sz.x + padding, 60 / 2.f - label_sz.y));
            auto pos_s = ImGui::GetCursorScreenPos();
            auto pos_l = ImGui::GetCursorPos();
            //ImGui::RenderFrame(pos_s + ImVec2{0, 0}, pos_s + ImVec2{50, 46}, ImColor(29, 46, 72), false, 0.f);
            //ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
            auto w = ImGui::CalcTextSize("00000").x;
            ImGui::PushItemWidth(w);
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::SetCursorPosX(pos_l.x + (50 - w) / 2.f);
            ImGui::Text(fmt::format("{:.3f}", val).c_str());
            ImGui::PopItemWidth();
        }

        bool layout_sign(const char* label, double& val, int col)
        {
            bool value_changed = false;
            ImGui::AlignTextToFramePadding();
            constexpr float input_w = 70;
            auto label_sz = ImGui::CalcTextSize("test");
            float ww = ImGui::GetColumnWidth(col);
            const float center_offset = (ww - input_w) / 2.f;
            auto start_pos = ImGui::GetCursorPos();
            auto pos_s = ImGui::GetCursorScreenPos();
            ImGui::SetCursorPosX(start_pos.x + center_offset);
            ImGui::SetCursorPosY(start_pos.y + 60 / 2.f - label_sz.y); //12 is magic value for width 30
            if(ImGui::Button(signToStr((Sign) val).data(), {70, 0}))
                ImGui::OpenPopup("sign_popup");
            if(ImGui::BeginPopup("sign_popup"))
            {
                for(int i = -1; i < 2; i++)
                    if(ImGui::Selectable(signToStr((Sign) i).data()))
                    {
                        val = i;
                        value_changed = true;
                    }
                ImGui::EndPopup();
            }
            return value_changed;
        }

    private:
        MatrixStorage<double> storage;
        std::vector<Magpie::Plot<float>> testArr;
    public:
        MagicInput() : storage(0, 0)
        {

        }

        void render() override
        {
            ImGui::Begin("Test Input");
            static std::vector<palka::Vec2<double>> points;
            static std::vector<Plot<double>> plots;
            static bool isEnd = false;
            static bool once = false;
            if(ImPlot::BeginPlot("test plot"))
            {
                if(ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(0) && ImGui::GetIO().KeyCtrl && !isEnd)
                {
                    auto pt = ImPlot::GetPlotMousePos();
                    auto pp = palka::Vec2<double>{roundoff(pt.x, 1), roundoff(pt.y, 1)};
                    if(points.size() > 2 && PlotCompare(points[0].x, pp.x, 0.01) && PlotCompare(points[0].y, pp.y, 0.01))
                    {
                        isEnd = true;
                        auto p1 = *(points.begin());
                        auto p2 = *(points.end() - 1);

                        if(p1.x > p2.x)
                            std::swap(p1, p2);
                        auto d1 = p2.y - p1.y;
                        auto d2 = p2.x - p1.x;
                        palka::Console::fmt_log("new line a={0} b={0}, c={0}", palka::Console::info, -d1, d2, d2 * p1.y - d1 * p1.x);
                        plots.emplace_back(-d1, d2, d2 * p1.y - d1 * p1.x);
                    } else
                    {
                        points.emplace_back(roundoff(pt.x, 1), roundoff(pt.y, 1));
                        if(points.size() > 1)
                        {
                            auto p1 = *(points.end() - 1);
                            auto p2 = *(points.end() - 2);

                            if(p1.x > p2.x)
                                std::swap(p1, p2);
                            auto d1 = p2.y - p1.y;
                            auto d2 = p2.x - p1.x;

                            palka::Console::fmt_log("new line a={0} b={0}, c={0}", palka::Console::info, -d1, d2, d2 * p1.y - d1 * p1.x);
                            plots.emplace_back(roundoff(-d1, 3), roundoff(d2, 3), roundoff(d2 * p1.y - d1 * p1.x, 3));
                        }
                    }
                }
                int index = 0;
                for(auto& p: points)
                {
                    if(index == 0)
                    {
                        ImPlot::SetNextMarkerStyle(ImPlotMarker_Square, 6, ImPlot::GetColormapColor(1), IMPLOT_AUTO, ImPlot::GetColormapColor(1));
                        ImPlot::PlotScatter(fmt::format("Start {0}", index++).c_str(), &p.x, &p.y, 1);
                    } else
                    {
                        ImPlot::SetNextMarkerStyle(ImPlotMarker_Circle, 4, ImPlot::GetColormapColor(3), IMPLOT_AUTO, ImPlot::GetColormapColor(3));
                        ImPlot::PlotScatter(fmt::format("Point {0}", index++).c_str(), &p.x, &p.y, 1);
                    }
                }
                if(isEnd)
                {
                    int plotIndex = 0;
                    for(int i = 0; i < points.size() - 1; ++i)
                    {
                        auto& p1 = points[i];
                        auto& p2 = points[i + 1];

                        if(once && (Sign) storage.get(2, plotIndex + 1) != Sign::EQUAL)
                        {
                            std::vector<double> x;
                            std::vector<double> y;
                            for(auto& p: plots[plotIndex].generateAAB(p1, 20))
                            {
                                x.emplace_back(p.x);
                                y.emplace_back(p.y);
                            }
                            for(auto& p: plots[plotIndex].generateAAB(p2, 20))
                            {
                                x.emplace_back(p.x);
                                y.emplace_back(p.y);
                            }
                            ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.15f);
                            switch((Sign) storage.get(2, plotIndex + 1))
                            {
                                case GREATEROREQUAL:
                                    ImPlot::PlotShaded(fmt::format("Shaded {0}", plotIndex).c_str(), x.data(), y.data(), x.size(), INFINITY);
                                    break;
                                case LESSOREQUAL:
                                    ImPlot::PlotShaded(fmt::format("Shaded {0}", plotIndex).c_str(), x.data(), y.data(), x.size(), -INFINITY);
                                    break;
                            }
                            ImPlot::PopStyleVar();
                        } else
                        {
                            double y[2] = {p1.y, p2.y};
                            double x[2] = {p1.x, p2.x};
                            ImPlot::PlotLine(fmt::format("{0}", i).c_str(), x, y, 2);
                        }
                        plotIndex++;
                    }
                    {
                        auto& p1 = points[points.size() - 1];
                        auto& p2 = points[0];

                        if(once && (Sign) storage.get(2, plotIndex + 1) != Sign::EQUAL)
                        {
                            std::vector<double> x;
                            std::vector<double> y;
                            for(auto& p: plots[plotIndex].generateAAB(p1, 20))
                            {
                                x.emplace_back(p.x);
                                y.emplace_back(p.y);
                            }
                            for(auto& p: plots[plotIndex].generateAAB(p2, 20))
                            {
                                x.emplace_back(p.x);
                                y.emplace_back(p.y);
                            }
                            ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.15f);
                            switch((Sign) storage.get(2, plotIndex + 1))
                            {
                                case GREATEROREQUAL:
                                    ImPlot::PlotShaded(fmt::format("Shaded {0}", plotIndex).c_str(), x.data(), y.data(), x.size(), INFINITY);
                                    break;
                                case LESSOREQUAL:
                                    ImPlot::PlotShaded(fmt::format("Shaded {0}", plotIndex).c_str(), x.data(), y.data(), x.size(), -INFINITY);
                                    break;
                            }
                            ImPlot::PopStyleVar();
                        } else
                        {
                            double y[2] = {p1.y, p2.y};
                            double x[2] = {p1.x, p2.x};
                            ImPlot::PlotLine(fmt::format("{0}", points.size()).c_str(), x, y, 2);
                        }
                    }
                } else
                    for(auto& p: plots)
                    {
                        double y[2] = {-5, 5};
                        double x[2] = {p.getValueAtX(-5), p.getValueAtX(5)};
                        ImPlot::PlotLine(fmt::format("plot a={:.2f} b={:.2f}, c={:.2f}", p.a, p.b, p.c).c_str(), x, y, 2);
                    }
                ImPlot::EndPlot();
                if(isEnd && !once)
                {
                    storage.alloc_matrix(plots.size() + 1, 4);
                    int row = 1;
                    for(auto& p: plots)
                    {
                        storage.get(0, row) = p.a;
                        storage.get(1, row) = p.b;
                        storage.get(2, row) = Sign::EQUAL;
                        storage.get(3, row) = p.c;
                        row++;
                    }
                    once = true;
                }
            }
            ImGui::End();
            if(isEnd)
            {
                if(ImGui::Begin("ASD"))
                {
                    ImGui::Text("f = c_1*x_1 + x_2*x_2 + ... + c_n*x_n");
                    if(ImGui::BeginTable("##table1", storage.columns_count() - 2, ImGuiTableFlags_SizingStretchProp))
                    {
                        ImGui::TableNextRow();
                        for(int column = 0; column < storage.columns_count() - 2; ++column)
                        {
                            ImGui::PushID(column);
                            ImGui::TableNextColumn();
                            std::string label = "X" + std::to_string(column);
                            ImGui::InputScalar(label.c_str(), ImGuiDataType_Double, &storage.get(column, 0));
                            ImGui::PopID();
                        }
                        ImGui::EndTable();
                    }
                    constexpr int cell_height = 60; //stores the height of the table element,
                    // this is a constant value, but it is different for different table elements
                    const float textPaddingY = 65.f;
                    auto curr_wnd = ImGui::GetCurrentWindow();
                    auto contentRect = curr_wnd->ContentRegionRect;
                    palka::Vec2f sz = {contentRect.GetWidth(), contentRect.GetHeight()};
                    //  ImVec2 center = contentRect.GetCenter();
                    ImVec2 center_local = contentRect.GetCenter() - curr_wnd->Pos;
                    ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
                    ImVec2 TextPos{center_local.x - ImGui::CalcTextSize("a_1*x_1 + a_2*x_2 + ... + a_n*x_n [>=,=,<=] B").x / 2.f, textPaddingY};
                    ImGui::SetCursorPos(TextPos);
                    ImGui::Text("a_1*x_1 + a_2*x_2 + ... + a_n*x_n [>=,=,<=] B");
                    ImVec2 barrier = TextPos + ImVec2{0, 80.f};

                    if(auto pos = center_local.y - cell_height * storage.rows_count() / 2.f; barrier.y < pos) //center, 3 -> row count
                        ImGui::SetCursorPosY(center_local.y - cell_height * storage.rows_count() / 2.f);
                    else
                        ImGui::SetCursorPosY(barrier.y);

                    int id = 0;
                    if(ImGui::BeginTable("Layout", storage.columns_count(), flags, {}))
                    {
                        for(int row = 1; row < storage.rows_count(); row++)
                        {
                            ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                            for(int column = 0; column < storage.columns_count() - 2; column++)
                            {
                                ImGui::TableNextColumn();

                                static int c = 0;
                                ImGui::PushID(id++);
                                layout2("asd", storage.get(column, row), column);
                                ImGui::PopID();

                            }
                            ImGui::TableNextColumn();
                            ImGui::PushID(id++);
                            layout_sign(signToStr((Sign) storage.get(storage.columns_count() - 2, row)).data(),
                                        storage.get(storage.columns_count() - 2, row), 0);

                            ImGui::TableNextColumn();
                            layout2("asd", storage.get(storage.columns_count() - 1, row), 4);
                            ImGui::PopID();
                        }

                        ImGui::EndTable();
                    }
                    if(ImGui::Button("next"))
                        sceneCallback();

                    ImGui::End();
                }
            }
        }

        void update() override
        {

        }

        void setEvents() override
        {

        }

        auto getResult()
        {
            return storage;
        }
    };
}

#endif //MAGPIE_MAGICINPUT_H
