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
#include "EventManager.h"
#include "config.h"

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
            auto label_sz = ImGui::CalcTextSize(label);
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
            ImGui::TextUnformatted(label);
            ImGui::SameLine();

            ImGui::SetCursorPos(start_pos + ImVec2(center_offset + label_sz.x + padding, 60 / 2.f - label_sz.y));
            auto pos_s = ImGui::GetCursorScreenPos();
            auto pos_l = ImGui::GetCursorPos();
            //ImGui::RenderFrame(pos_s + ImVec2{0, 0}, pos_s + ImVec2{50, 46}, ImColor(29, 46, 72), false, 0.f);
            //ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
            auto w = ImGui::CalcTextSize("0.00").x;
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
            ImGui::SetCursorPosX(start_pos.x + center_offset);
            ImGui::SetCursorPosY(start_pos.y + 60 / 2.f - label_sz.y);
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
        std::vector<Plot<double>> plots;
        std::vector<palka::Vec2<double>> points;
        bool SetIsClosed;
        bool StorageOnceInit;

        struct Union
        {
            std::vector<double> x;
            std::vector<double> y;
        };
        std::map<int, Union> UnionPoints;

        void drawUnion(int plotIndex)
        {
            ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.15f);
            switch((Sign) storage.get(2, plotIndex + 1))
            {
                case GREATEROREQUAL:
                    ImPlot::PlotShaded(fmt::format("Plot {0}", plotIndex).c_str(), UnionPoints[plotIndex].x.data(),
                                       UnionPoints[plotIndex].y.data(), UnionPoints[plotIndex].x.size(), INFINITY);
                    break;
                case LESSOREQUAL:
                    ImPlot::PlotShaded(fmt::format("Plot {0}", plotIndex).c_str(), UnionPoints[plotIndex].x.data(),
                                       UnionPoints[plotIndex].y.data(), UnionPoints[plotIndex].x.size(), -INFINITY);
                    break;
                case EQUAL:
                    break;
            }
            ImPlot::PopStyleVar();
        }

        void drawLine(const palka::Vec2<double>& p1, const palka::Vec2<double>& p2, int index)
        {
            double y[2] = {p1.y, p2.y};
            double x[2] = {p1.x, p2.x};
            ImPlot::PlotLine(fmt::format("Plot {0}", index).c_str(), x, y, 2);
        }

        void initStorage()
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
            int plotIndex = 0;
            for(int i = 0; i < points.size(); ++i)
            {
                auto& p1 = points[i];
                int j = (i + 1 < points.size()) ? i + 1 : 0;
                auto& p2 = points[j];
                std::vector<double> x;
                std::vector<double> y;
                Union u{};
                if(plots[plotIndex].b == 0)
                {
                } else
                {
                    for(auto& p: plots[plotIndex].generateAAB2(p1, 20))
                    {
                        u.x.emplace_back(p.x);
                        u.y.emplace_back(p.y);
                    }
                    for(auto& p: plots[plotIndex].generateAAB2(p2, 20))
                    {
                        u.x.emplace_back(p.x);
                        u.y.emplace_back(p.y);
                    }
                }
                UnionPoints.emplace(plotIndex, u);
                plotIndex++;
            }
        }

    public:
        MagicInput() : storage(0, 0), SetIsClosed(false), StorageOnceInit(false)
        {}

        MagicInput(std::string_view name, palka::Vec2f size)
                : UiView(name, size), storage(0, 0), SetIsClosed(false), StorageOnceInit(false)
        {}

        void render() override
        {
            ImGui::SetNextWindowPos(ImVec2((Config::WindowSize.x - (size.x)) / 2,
                                           (Config::WindowSize.y - (size.y)) / 2), ImGuiCond_Always, {0, 0});
            if(ImGui::Begin("Magic Input"))
            {
                ImGui::SetWindowSize({size.x, size.y});
                if(ImPlot::BeginPlot("Create union"))
                {
                    if(ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(0) && ImGui::GetIO().KeyCtrl && !SetIsClosed)
                    {
                        auto pt = ImPlot::GetPlotMousePos();
                        auto pp = palka::Vec2<double>{roundoff(pt.x, 1), roundoff(pt.y, 1)};
                        if(points.size() > 2 && PlotCompare(points[0].x, pp.x, 0.01) && PlotCompare(points[0].y, pp.y, 0.01))
                        {
                            SetIsClosed = true;
                            auto p1 = *(points.begin());
                            auto p2 = *(points.end() - 1);

                            if(p1.x > p2.x)
                                std::swap(p1, p2);
                            auto d1 = p2.y - p1.y;
                            auto d2 = p2.x - p1.x;
                            palka::Console::fmt_log("plot a={:.2f} b={:.2f}, c={:.2f}", palka::Console::info, -d1, d2, d2 * p1.y - d1 * p1.x);
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

                                palka::Console::fmt_log("plot a={:.2f} b={:.2f}, c={:.2f}", palka::Console::info, -d1, d2, d2 * p1.y - d1 * p1.x);
                                plots.emplace_back(roundoff(-d1, 3), roundoff(d2, 3), roundoff(d2 * p1.y - d1 * p1.x, 3));
                            }
                        }
                    }
                    int index = 0;
                    if(SetIsClosed) //draw Union
                    {
                        int plotIndex = 0;
                        for(int i = 0; i < points.size(); ++i)
                        {
                            auto& p1 = points[i];
                            int j = (i + 1 < points.size()) ? i + 1 : 0;
                            auto& p2 = points[j];
                            if(StorageOnceInit && (Sign) storage.get(2, plotIndex + 1) != Sign::EQUAL)
                            {
                                plots[plotIndex].sign = (Sign) storage.get(2, plotIndex + 1);
                                if(plots[plotIndex].b == 0) //on the X axis, the function cannot be scaled to infinity, so every time (perhaps the sign changes)
                                    // we calculate a new area
                                {
                                    std::vector<double> x;
                                    std::vector<double> y;
                                    Union u{};

                                    u.x.emplace_back(p1.x);
                                    u.y.emplace_back(p1.y - 100);
                                    u.x.emplace_back(p1.x);
                                    u.y.emplace_back(p1.y + 100);

                                    if(plots[plotIndex].containsPoint(p1 - palka::Vec2<double>{5.0, 0.0}))
                                    {
                                        u.y.emplace_back(p1.y + 100);
                                        u.x.emplace_back(p1.x - 30);
                                        u.y.emplace_back(p1.y + 100);
                                        u.x.emplace_back(p1.x - 30);
                                    } else
                                    {
                                        u.y.emplace_back(p1.y + 100);
                                        u.x.emplace_back(p1.x + 30);
                                        u.y.emplace_back(p1.y + 100);
                                        u.x.emplace_back(p1.x + 30);
                                    }
                                    UnionPoints[plotIndex] = u;
                                    ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.15f);
                                    ImPlot::PlotShaded(fmt::format("Plot {0}", plotIndex).c_str(), UnionPoints[plotIndex].x.data(),
                                                       UnionPoints[plotIndex].y.data(), UnionPoints[plotIndex].x.size(), -INFINITY);
                                    ImPlot::PopStyleVar();
                                } else
                                {
                                    drawUnion(plotIndex);
                                }
                            } else
                                drawLine(p1, p2, plotIndex);
                            plotIndex++;
                        }
                    } else //Draw plots
                    {
                        for(auto& p: plots)
                        {
                            double y[2] = {-5, 5};
                            double x[2] = {p.getValueAtX(-5), p.getValueAtX(5)};
                            if(p.a == 0)
                            {
                                x[0] = -5;
                                x[1] = 5;
                                y[0] = p.getValueAtY(-5);
                                y[1] = p.getValueAtY(5);
                            }
                            ImPlot::PlotLine(fmt::format("plot a={:.2f} b={:.2f}, c={:.2f}", p.a, p.b, p.c).c_str(), x, y, 2);
                        }
                    }
                    for(auto& p: points) //draw points (always)
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
                    ImPlot::EndPlot();
                }

                if(SetIsClosed && !StorageOnceInit) //when the set is closed the storage is initialized (once)
                {
                    initStorage();
                    StorageOnceInit = true;
                }

                if(SetIsClosed) //when set is closed
                {
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40.f);
                    if(ImGui::BeginChild("Edit result"))
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
                        int id = 0;
                        if(ImGui::BeginTable("Layout", storage.columns_count(), flags, {}))
                        {
                            for(int row = 1; row < storage.rows_count(); row++)
                            {
                                ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);

                                ImGui::TableNextColumn();
                                ImGui::PushID(id++);
                                layout2("x", storage.get(0, row), 0);
                                ImGui::PopID();

                                ImGui::TableNextColumn();
                                ImGui::PushID(id++);
                                layout2("y", storage.get(1, row), 0);
                                ImGui::PopID();

                                ImGui::TableNextColumn();
                                ImGui::PushID(id++);
                                layout_sign(signToStr((Sign) storage.get(storage.columns_count() - 2, row)).data(),
                                            storage.get(storage.columns_count() - 2, row), 0);
                                ImGui::TableNextColumn();
                                layout2(" ", storage.get(storage.columns_count() - 1, row), 4);
                                ImGui::PopID();
                            }

                            ImGui::EndTable();
                        }
                        if(ImGui::Button("next"))
                            sceneCallback();

                        ImGui::EndChild();
                    }
                }
                ImGui::End();
            }
        }

        void update() override
        {}

        void setEvents() override
        {
            palka::EventManager::addEvent(palka::EventType::KEYDOWN, [this](palka::EventData e)
            {
                if(palka::EventManager::isKeyPressed(GLFW_KEY_LEFT_CONTROL) && palka::EventManager::isKeyPressed(GLFW_KEY_Z))
                {
                    if(points.size() > 1 && !SetIsClosed)
                    {
                        points.pop_back();
                        plots.pop_back();
                    }
                }
            });
        }

        auto getResult()
        {
            return storage;
        }
    };
}

#endif //MAGPIE_MAGICINPUT_H
