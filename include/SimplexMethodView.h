#ifndef MAGPIE_SIMPLEXMETHODVIEW_H
#define MAGPIE_SIMPLEXMETHODVIEW_H

#include "UiView.h"
#include "config.h"
#include "SimplexMethod.h"
#include "SolverMemento.h"
#include "ArtificialBasis.h"
#include "imgui_internal.h"
#include <imgui.h>
#include <regex>

namespace Magpie
{
    template<class T>
    class SimplexMethodView : public UiView
    {
    private:
        //SimplexMethod<double> s;
        SimplexMethod2<T> s2;
        ArtificialBasis<T> artificialBasis;
        MementoHistory<SimplexMethod2<T>> h;
        MementoHistory<SimplexResultIterative2<T>> hist_result;
        SimplexResultIterative2<T> result;

        template<typename... Args>
        static void ShowText(const std::string& rt_fmt_str, Args&& ... args)
        {
            ImGui::Text(vformat(rt_fmt_str, fmt::make_format_args(args...)).c_str());
        }

        void simplex_table_render()
        {
            constexpr int cell_height = 80;
            ImGuiTableFlags flags = ImGuiTableFlags_Borders;
            auto changed_row_color = ImVec4(0.9, 0.322, 0.375, 0.9);
            auto changed_col_color = ImVec4(0.2, 0.922, 0.375, 0.9);
            auto resolving_elem_color = ImVec4(0.4, 0.522, 0.775, 0.9);
            auto table_sz = ImVec2(cell_height * result.after.columns_count(), static_cast<float>(cell_height * (result.after.rows_count() + 1)));
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - table_sz.x) / 2.f);
            ImU32 cell_bg_color = ImGui::GetColorU32(ImGuiCol_TableHeaderBg);
            if(ImGui::BeginTable("SimplexAfter", result.after.columns_count() + 1, flags, table_sz))
            {
                for(int i = 0; i < result.after.rows_count(); ++i)
                {
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                    if(i == 0)
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text(" ");
                        for(int j = 0; j < result.after.columns_count() - 1; ++j)
                        {
                            ImGui::TableNextColumn();
                            layout2("x%s", std::to_string(j), j, cell_height);
                            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                        }
                        ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                    }

                    if(i < result.after.rows_count() - 1)
                    {
                        ImGui::TableNextColumn();
                        layout2("x%s", std::to_string(result.basis[i]), i, cell_height);
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                    } else
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text(" ");
                    }
                    for(int j = 0; j < result.after.columns_count(); ++j)
                    {
                        ImGui::TableNextColumn();
                        if(i == result.resolving_row && j != result.resolving_col)
                        {
                            ImU32 cell_bg_color = ImGui::GetColorU32(changed_row_color);
                            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                        } else if(i != result.resolving_row && j == result.resolving_col)
                        {
                            ImU32 cell_bg_color = ImGui::GetColorU32(changed_col_color);
                            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                        } else if(i == result.resolving_row && j == result.resolving_col)
                        {
                            ImU32 cell_bg_color = ImGui::GetColorU32(resolving_elem_color);
                            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                        }
                        ShowText("{} (?)", result.after.get(j, i));
                        //ImGui::Text("%s (?)", result.after.get(j, i));
                        if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
                        {
                            ImGui::BeginTooltip();
                            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                            if(i == result.resolving_row && j == result.resolving_col)
                            {
                                ShowText("1 / {} = {} ", result.before.get(result.resolving_col, result.resolving_row),
                                         result.after.get(j, i));
//                                ImGui::Text("%s", fmt::format("1 / {} = {} ", result.before.get(result.resolving_col, result.resolving_row),
//                                                              result.after.get(j, i)).c_str());
                            } else if(i == result.resolving_row)
                            {
                                ShowText("{} / {} = {}", result.before.get(i, result.resolving_row),
                                         result.before.get(result.resolving_col, result.resolving_row),
                                         result.after.get(i, result.resolving_row));
//                                ImGui::Text("%s / %s = %s", result.before.get(i, result.resolving_row),
//                                            result.before.get(result.resolving_col, result.resolving_row),
//                                            result.after.get(i, result.resolving_row));
                            } else if(j == result.resolving_col)
                            {
                                ShowText("{} / {} * -1 = {}", result.before.get(result.resolving_col, i),
                                         result.before.get(result.resolving_col, result.resolving_row),
                                         result.after.get(result.resolving_col, i));
//                                ImGui::Text("%s / %s * -1 = %s", result.before.get(result.resolving_col, i),
//                                            result.before.get(result.resolving_col, result.resolving_row),
//                                            result.after.get(result.resolving_col, i));
                            } else
                            {
                                ShowText("{} - {} * {} = {} ", result.before.get(j, i),
                                         result.after.get(j, result.resolving_row),
                                         result.before.get(result.resolving_col, i),
                                         result.after.get(j, i));
//                                ImGui::Text("%s",
//                                            fmt::format("{} - {} * {} = {} ", result.before.get(j, i),
//                                                        result.after.get(j, result.resolving_row),
//                                                        result.before.get(result.resolving_col, i),
//                                                        result.after.get(j, i)).c_str());
                            }

                            ImGui::PopTextWrapPos();
                            ImGui::EndTooltip();
                        }
                    }
                }
                ImGui::EndTable();
            }
        }

        void select_support_render()
        {
            constexpr int cell_height = 80;
            constexpr int cell_width = 80;
            ImGui::Text("Select support element: ");
            ImGuiTableFlags flags = ImGuiTableFlags_Borders;
            static int selected = -1;
            auto table_sz = ImVec2(cell_width * result.after.columns_count(), static_cast<float>(cell_height * (result.after.rows_count() + 1)));
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - table_sz.x) / 2.f);
            ImU32 cell_bg_color = ImGui::GetColorU32(ImGuiCol_TableHeaderBg);
            if(ImGui::BeginTable("##SimplexAfter", result.after.columns_count() + 1, flags, table_sz))
            {
                for(int i = 0; i < result.after.rows_count(); ++i)
                {
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                    if(i == 0)
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text(" ");
                        for(int j = 0; j < result.after.columns_count() - 1; ++j)
                        {
                            ImGui::TableNextColumn();
                            layout2("x%s", std::to_string(j), j, cell_height);
                            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                        }
                        ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                    }

                    if(i < result.after.rows_count() - 1)
                    {
                        ImGui::TableNextColumn();
                        layout2("x%s", std::to_string(result.basis[i]), i, cell_height);
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                    } else
                    {
                        ImGui::TableNextColumn();
                        ImGui::Text(" ");
                    }

                    for(int j = 0; j < result.after.columns_count(); ++j)
                    {
                        ImGui::TableNextColumn();
                        if(result.possibleSupportElems.contains(j) && result.possibleSupportElems[j] == i)
                        {
                            if(layout_selectable(fmt::format("{}", result.after.get(j, i)).c_str(), cell_height, 120))
                            {
                                selected = j;
                                s2.setSupportElem(i, j);
                            }
                            if(selected == j)
                            {
                                ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(0.9, 0.322, 0.375, 0.9));
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                            } else if(i == result.bestSupportElem.first && j == result.bestSupportElem.second)
                            {
                                ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(0.298, 0.922, 0.175, 0.9));
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                            } else
                            {
                                ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(0.498, 0.322, 0.875, 0.9));
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                            }
                        } else
                        {
                            layout2("%s", result.after.get(j, i), i, cell_height);
                        }
                    }
                }
                ImGui::EndTable();
            }
        }


        void result_render()
        {
            ImGui::Text("Table");
            ImGui::Spacing();
            ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
            constexpr int cell_height = 80;
            auto table_sz = ImVec2(cell_height * result.after.columns_count(), static_cast<float>(cell_height * result.after.rows_count()));
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - table_sz.x) / 2.f);
            if(ImGui::BeginTable("SimplexAfter", result.after.columns_count(), flags, table_sz))
            {
                int id = 0;
                for(int i = 0; i < result.after.rows_count(); ++i)
                {
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                    for(int j = 0; j < result.after.columns_count(); ++j)
                    {
                        ImGui::TableNextColumn();
                        ImGui::PushID(id++);
                        layout2("%s", result.after.get(j, i), j, cell_height);
                        ImGui::PopID();
                    }
                }
                ImGui::EndTable();
            }
        }

        void table_render()
        {
            ImGui::Text("Table");
            ImGui::Spacing();
            ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
            constexpr int cell_height = 80;
            auto table_sz = ImVec2(cell_height * result.after.columns_count(), static_cast<float>(cell_height * result.after.rows_count()));
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - table_sz.x) / 2.f);
            if(ImGui::BeginTable("SimplexAfter", result.after.columns_count(), flags, table_sz))
            {
                int id = 0;
                for(int i = 0; i < result.after.rows_count(); ++i)
                {
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                    for(int j = 0; j < result.after.columns_count(); ++j)
                    {
                        ImGui::TableNextColumn();
                        ImGui::PushID(id++);
                        layout2("%s", result.after.get(j, i), j, cell_height);
                        ImGui::PopID();
                    }
                }
                ImGui::EndTable();
            }
        }

        void history_render()
        {

        }

        void input_r()
        {
            ImGui::SetCursorPosY(40);
            constexpr int cell_height = 80;
            std::string func = fmt::format("{}x_{}", result.after.get(0, 0), 1);
            for(int column = 1; column < result.after.columns_count() - 2; ++column)
            {
                if(compare_float<Fractus>(result.after.get(column, 0), 1))
                    func += fmt::format(" + x_{}", column + 1);
                else if(compare_float<Fractus>(result.after.get(column, 0), -1))
                    func += fmt::format(" - x_{}", column + 1);
                else if(result.after.get(column, 0) < 0)
                    func += fmt::format(" - {}x_{}", abs(result.after.get(column, 0)), column + 1);
                else
                    func += fmt::format(" + {}x_{}", result.after.get(column, 0), column + 1);
            }
            func = fmt::format("Objective function: {}", func.c_str());
            auto func_bb = ImGui::CalcTextSize(func.c_str());
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - func_bb.x) / 2.f);
            ImGui::Text("%s", func.c_str());
            ImGui::Spacing();
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Limitations").x) / 2.f);
            ImGui::Text("Limitations");
            ImGui::Spacing();
            ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_NoHostExtendY;
            auto table_sz = ImVec2(cell_height * result.after.columns_count(), static_cast<float>(cell_height * (result.after.rows_count() - 1)));
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - table_sz.x) / 2.f);
            if(ImGui::BeginTable("##InputTable", result.after.columns_count(), flags, table_sz))
            {
                int id = 0;
                for(int row = 1; row < result.after.rows_count(); row++)
                {
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                    for(int column = 0; column < result.after.columns_count() - 2; column++)
                    {
                        ImGui::TableNextColumn();
                        ImGui::PushID(id++);
                        layout2("%s", result.after.get(column, row), column, cell_height);
                        ImGui::PopID();
                    }
                    ImGui::TableNextColumn();
                    ImGui::PushID(id++);
                    layout_sign((int) result.after.get(result.after.columns_count() - 2, row), 0, cell_height);
                    ImGui::TableNextColumn();
                    layout2("%s", result.after.get(result.after.columns_count() - 1, row), 4, cell_height);
                    ImGui::PopID();
                }

                ImGui::EndTable();
            }
        }

        void layout2(std::string_view fmt, std::string val, int col, int col_h)
        {
            auto size = ImGui::CalcTextSize(val.c_str());
            float ww = ImGui::GetColumnWidth(col);
            auto start_pos = ImGui::GetCursorPos();
            ImGui::SetCursorPosY(start_pos.y + (col_h - size.y) / 2.f);
            ImGui::SetCursorPosX(start_pos.x + (ww - size.x) / 2.f);
            ImGui::Text(fmt.data(), val.c_str());
        }

        void layout2(std::string_view fmt, double val, int col, int col_h)
        {
            auto size = ImGui::CalcTextSize("0.002");
            float ww = ImGui::GetColumnWidth(col);
            auto start_pos = ImGui::GetCursorPos();
            ImGui::SetCursorPosY(start_pos.y + (col_h - size.y) / 2.f);
            ImGui::SetCursorPosX(start_pos.x + (ww - size.x) / 2.f);
            ImGui::Text("%.3f", val);
        }

        bool layout_selectable(std::string_view value, int cell_height, int cell_width)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.2f, 0.5f));
            auto ret = ImGui::Selectable(value.data(), false, ImGuiItemFlags_None, ImVec2(cell_width, cell_height));
            ImGui::PopStyleVar();
            ImGui::PopStyleVar();
            return ret;
        }

        void layout_sign(int val, int col,int col_h)
        {
            auto ssign = signToStr((Sign) val);
            auto size = ImGui::CalcTextSize(ssign.data());
            float ww = ImGui::GetColumnWidth(col);
            auto start_pos = ImGui::GetCursorPos();
            ImGui::SetCursorPosY(start_pos.y + (col_h - size.y) / 2.f);
            ImGui::SetCursorPosX(start_pos.x + (ww - size.x) / 2.f);
            ImGui::Text(ssign.data());
        }


    public:
        SimplexMethodView(SimplexMethodView&&) = default;

        SimplexMethodView& operator=(SimplexMethodView&&) noexcept = default;

        void init() override
        {
            std::vector<size_t> basis;
            result = s2.init(storage->getData<T>(), basis);
            h.update(s2);
            hist_result.update(result);
        }
        
        SimplexMethodView(palka::Vec2f size, bool open = true,
                          ImGuiWindowFlags w_flag = ImGuiWindowFlags_None)
                : UiView("SimplexMethodView", size, open, w_flag)
        {
//            artificialBasis.init(store);
//            artificialBasis.simplex_method();
//            artificialBasis.simplex_method();
//            artificialBasis.simplex_method();

        }

        SimplexMethodView(palka::Vec2f size, MatrixStorage<Fractus> store, std::vector<size_t> basis, bool open = true,
                          ImGuiWindowFlags w_flag = ImGuiWindowFlags_None)
                : UiView("SimplexMethodView", size, open, w_flag)
        {
//            artificialBasis.init(store);
//            artificialBasis.simplex_method();
//            artificialBasis.simplex_method();
//            artificialBasis.simplex_method();
//            result = s2.init(store, basis);
//            h.update(s2);
//            hist_result.update(result);
        }

        ~SimplexMethodView() override = default;

        void render(palka::Window& w) override
        {
        }

        void update() override
        {
            ImGui::SetNextWindowPos(ImVec2((Config::WindowSize.x - (size.x)) / 2,
                                           (Config::WindowSize.y - (size.y)) / 2), ImGuiCond_Always, {0, 0});
            if(ImGui::Begin(name.c_str(), &open, win_flag))
            {
                ImGui::SetWindowSize({size.x, size.y});
                if(ImGui::BeginChild("##MainRegion", ImVec2(size.x - 40.f, size.y * 0.86f), false,
                                     ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoTitleBar |
                                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                                     ImGuiWindowFlags_NoBackground))
                {
                    switch(result.state)
                    {
                        case SiMetResultType::SIMPLEX_TABLE:
                            simplex_table_render();
                            break;
                        case SiMetResultType::INPUT_DATA:
                            input_r();
                            break;
                        case SiMetResultType::SELECT_SUPPORT_ELEM:
                            select_support_render();
                            break;
                        case SiMetResultType::TABLE:
                            table_render();
                            break;
                        case SiMetResultType::RESULT_TABLE:
                            result_render();
                            break;
                    }
                    ImGui::EndChild();
                }
                if(ImGui::BeginChild("##ControlRegion", {0, 45.f}, false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoTitleBar |
                                                                          ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                                                                          ImGuiWindowFlags_NoBackground))
                {
                    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 248.f) / 2.f);
                    if(ImGui::Button("Next iteration", {120, 35}))
                    {
                        result = s2.solve_iterative();
                        hist_result.update(result);
                        h.update(s2);
                    }
                    ImGui::SameLine();
                    if(ImGui::Button("Back", {120, 35}))
                    {
                        h.undo(s2);
                        hist_result.undo(result);
                    }
                    ImGui::EndChild();
                }
                ImGui::End();
            }
        }

        void setEvents() override
        {}

    };
}

#endif //MAGPIE_SIMPLEXMETHODVIEW_H
