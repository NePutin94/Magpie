#ifndef MAGPIE_MAGICINPUT_H
#define MAGPIE_MAGICINPUT_H

#include "UiView.h"
#include <implot.h>
#include <fmt/format.h>
#include "GraphicsMethod2D.h"
#include <EventManager.h>
#include "config.h"
#include <ConsoleLog.h>
#include "../lib/imgui/implot_internal.h"

namespace Magpie
{
    class MagicInput : public UiView
    {
        inline int AxisPrecision(const ImPlotAxis& axis)
        { //check implot.cpp
            const double range = axis.Ticker.TickCount() > 1 ? (axis.Ticker.Ticks[1].PlotPos - axis.Ticker.Ticks[0].PlotPos) : axis.Range.Size();
            return ImPlot::Precision(range);
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
        MatrixStorage<double> MyStorage;
        Plot<double> Func;
        std::vector<Plot<double>> plots;
        std::vector<palka::Vec2<double>> points;
        bool SetIsClosed;
        bool StorageOnceInit;

        struct Union
        {
            std::vector<double> x;
            std::vector<double> y;

            std::vector<double> x2; //for the case plot.b==0
            std::vector<double> y2;
        };
        std::map<int, Union> UnionPoints;

        void drawUnion(int plotIndex)
        {
            if(plots[plotIndex].b == 0)
            {
                ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.15f);
                switch((Sign) MyStorage.get(2, plotIndex + 1))
                {
                    case GREATEROREQUAL:
                        ImPlot::PlotShaded(fmt::format("Plot {0}", plotIndex).c_str(), UnionPoints[plotIndex].x.data(),
                                           UnionPoints[plotIndex].y.data(), UnionPoints[plotIndex].x.size(), -INFINITY);
                        break;
                    case LESSOREQUAL:
                        ImPlot::PlotShaded(fmt::format("Plot {0}", plotIndex).c_str(), UnionPoints[plotIndex].x2.data(),
                                           UnionPoints[plotIndex].y2.data(), UnionPoints[plotIndex].x.size(), -INFINITY);
                        break;
                    case EQUAL:
                        break;
                }
                ImPlot::PopStyleVar();
            } else
            {
                ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.15f);
                switch((Sign) MyStorage.get(2, plotIndex + 1))
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
        }

        void drawLine(const palka::Vec2<double>& p1, const palka::Vec2<double>& p2, int index)
        {
            double y[2] = {p1.y, p2.y};
            double x[2] = {p1.x, p2.x};
            ImPlot::PlotLine(fmt::format("Plot {0}", index).c_str(), x, y, 2);
        }

        void initStorage()
        {
            MyStorage.alloc_matrix(plots.size() + 1, 4);
            int row = 1;
            for(auto& p: plots)
            {
                MyStorage.get(0, row) = p.a;
                MyStorage.get(1, row) = p.b;
                MyStorage.get(2, row) = Sign::EQUAL;
                MyStorage.get(3, row) = p.c;
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
                    std::vector<double> x;
                    std::vector<double> y;
                    Union u{};

                    u.x.emplace_back(p1.x);
                    u.y.emplace_back(p1.y - 300);
                    u.x.emplace_back(p1.x);
                    u.y.emplace_back(p1.y + 300);

                    u.y.emplace_back(p1.y + 300);
                    u.x.emplace_back(p1.x + 300);
                    u.y.emplace_back(p1.y + 300);
                    u.x.emplace_back(p1.x + 300);

                    u.x2.emplace_back(p1.x);
                    u.y2.emplace_back(p1.y - 300);
                    u.x2.emplace_back(p1.x);
                    u.y2.emplace_back(p1.y + 300);
                    u.y2.emplace_back(p1.y + 300);
                    u.x2.emplace_back(p1.x - 300);
                    u.y2.emplace_back(p1.y + 300);
                    u.x2.emplace_back(p1.x - 300);
                    UnionPoints[plotIndex] = u;
                } else
                {
                    for(auto& p: plots[plotIndex].generateAAB2(p1, 90))
                    {
                        u.x.emplace_back(p.x);
                        u.y.emplace_back(p.y);
                    }
//                    for(auto& p: plots[plotIndex].generateAAB2(p2, 20))
//                    {
//                        u.x.emplace_back(p.x);
//                        u.y.emplace_back(p.y);
//                    }
                }
                UnionPoints.emplace(plotIndex, u);
                plotIndex++;
            }
        }

        void navLayout(bool isReady)
        {
            constexpr int b_padding = 40.f;
            ImVec2 b_size{80.f, 35.f};
            ImGui::Spacing();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x * .5f);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - b_size.x - b_padding / 2.f);
            if(ImGui::Button("Back", b_size))
            {
                nextSatet = States::Back;
                sceneCallback();
            }
            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + b_padding / 2.f);
            if(!isReady)
            {
                ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
            }
            if(ImGui::Button("Next", b_size))
            {
                storage->data.alloc_matrix(MyStorage.rows_count(), MyStorage.columns_count());
                for(int i = 0; i < MyStorage.rows_count(); ++i)
                    for(int j = 0; j < MyStorage.columns_count(); ++j)
                        storage->data.get(j, i) = MyStorage.get(j, i);
                nextSatet = States::Menu;
                sceneCallback();
            }
            if(!isReady)
            {
                ImGui::PopItemFlag();
                ImGui::PopStyleVar();
            }
        }

        palka::Vec2f scale;
    public:
        MagicInput() : MyStorage(0, 0), SetIsClosed(false), StorageOnceInit(false)
        {}

        MagicInput(std::string_view name, palka::Vec2f scale)
                : UiView(name, Config::WindowSize * scale), MyStorage(0, 0), SetIsClosed(false), StorageOnceInit(false), scale(scale)
        {}

        void render(palka::Window& w) override
        {
            size = Config::WindowSize * scale;
            ImGui::SetNextWindowPos(ImVec2((Config::WindowSize.x - (size.x)) / 2,
                                           (Config::WindowSize.y - (size.y)) / 2), ImGuiCond_Always, {0, 0});
            if(ImGui::Begin("Magic Input"))
            {
                ImGui::SetWindowSize(ImVec2{size.x, size.y});
                static int prec = 1;
                if(!SetIsClosed)
                    ImGui::DragInt("Precision", &prec, 0.4f, 0, 4);
                else
                    ImGui::DragScalar("C", ImGuiDataType_::ImGuiDataType_Double, reinterpret_cast<void*>(&Func.c), 1 / std::pow(10, (double) prec));

                if(ImPlot::BeginPlot("Create union", {-1.f, size.y * 0.6f}))
                {
                    if(ImPlot::IsPlotHovered() && ImGui::IsMouseClicked(0) && ImGui::GetIO().KeyCtrl && !SetIsClosed)
                    {
                        auto pt = ImPlot::GetPlotMousePos();
                        auto pp = palka::Vec2<double>{ImPlot::RoundTo(pt.x, prec), ImPlot::RoundTo(pt.y, prec)};
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
                            points.emplace_back(ImPlot::RoundTo(pt.x, prec), ImPlot::RoundTo(pt.y, prec));
                            if(points.size() > 1)
                            {
                                auto p1 = *(points.end() - 1);
                                auto p2 = *(points.end() - 2);

                                if(p1.x > p2.x)
                                    std::swap(p1, p2);
                                auto d1 = p2.y - p1.y;
                                auto d2 = p2.x - p1.x;

                                palka::Console::fmt_log("plot a={:.2f} b={:.2f}, c={:.2f}", palka::Console::info, -d1, d2, d2 * p1.y - d1 * p1.x);
                                plots.emplace_back(-d1, d2, d2 * p1.y - d1 * p1.x);
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
                            if(StorageOnceInit && (Sign) MyStorage.get(2, plotIndex + 1) != Sign::EQUAL)
                            {
                                plots[plotIndex].sign = (Sign) MyStorage.get(2, plotIndex + 1);
                                drawUnion(plotIndex);
                            } else
                                drawLine(p1, p2, plotIndex);
                            plotIndex++;
                        }
                        double x[2] = {-10, 10};
                        double y[2] = {Func.getValueAtY(-10), Func.getValueAtY(10)};
                        ImPlot::PlotLine("Func", x, y, 2);
                    } else //Draw plots
                    {
                        for(auto& p: plots)
                        {
                            double y[2] = {-100, 100};
                            double x[2] = {p.getValueAtX(-100), p.getValueAtX(100)};
                            if(p.a == 0)
                            {
                                x[0] = -100;
                                x[1] = 100;
                                y[0] = p.getValueAtY(-100);
                                y[1] = p.getValueAtY(100);
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
                    constexpr int cell_height = 60; //stores the height of the table element,
                    // this is a constant value, but it is different for different table elements
                    const float textPaddingY = 65.f;
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40.f);
                    auto table_sz = ImVec2{MyStorage.columns_count() * 150.f, static_cast<float>((MyStorage.rows_count() - 1) * cell_height)};
                    if(ImGui::BeginChild("Edit result", {0, table_sz.y + 90}))
                    {
                        ImGui::Text("f = c_1*x_1 + x_2*x_2 + ... + c_n*x_n");
                        if(ImGui::BeginTable("##table1", MyStorage.columns_count() - 2, ImGuiTableFlags_SizingStretchProp))
                        {
                            ImGui::TableNextRow();
                            for(int column = 0; column < MyStorage.columns_count() - 2; ++column)
                            {
                                ImGui::PushID(column);
                                ImGui::TableNextColumn();
                                std::string label = "X" + std::to_string(column);
                                if(ImGui::InputScalar(label.c_str(), ImGuiDataType_Double, &MyStorage.get(column, 0)))
                                {
                                    Func.a = MyStorage.get(0, 0);
                                    Func.b = MyStorage.get(1, 0);
                                }
                                ImGui::PopID();
                            }
                            ImGui::EndTable();
                        }

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
                        if(ImGui::BeginTable("Layout", MyStorage.columns_count(), flags))
                        {
                            for(int row = 1; row < MyStorage.rows_count(); row++)
                            {
                                ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);

                                ImGui::TableNextColumn();
                                ImGui::PushID(id++);
                                layout2("x", MyStorage.get(0, row), 0);
                                ImGui::PopID();

                                ImGui::TableNextColumn();
                                ImGui::PushID(id++);
                                layout2("y", MyStorage.get(1, row), 0);
                                ImGui::PopID();

                                ImGui::TableNextColumn();
                                ImGui::PushID(id++);
                                layout_sign(signToStr((Sign) MyStorage.get(MyStorage.columns_count() - 2, row)).data(),
                                            MyStorage.get(MyStorage.columns_count() - 2, row), 0);
                                ImGui::TableNextColumn();
                                layout2("c", MyStorage.get(MyStorage.columns_count() - 1, row), 4);
                                ImGui::PopID();
                            }

                            ImGui::EndTable();
                        }
                        ImGui::EndChild();
                    }
                }
                navLayout(SetIsClosed);
                ImGui::End();
            }
        }

        void update() override
        {}

        void setEvents() override
        {
            palka::EventManager::addEvent(palka::_EventType::KEYDOWN, [this](palka::EventData e)
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
    };
}

#endif //MAGPIE_MAGICINPUT_H
