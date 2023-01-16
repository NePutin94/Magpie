#ifndef MAGPIE_SIMPLEXMETHODVIEW_H
#define MAGPIE_SIMPLEXMETHODVIEW_H

#include "UiView.h"
#include "config.h"
#include "SimplexMethod.h"
#include "SolverMemento.h"
#include "ArtificialBasis.h"
#include "imgui_internal.h"
#include <imgui.h>
#include <fmt/core.h>

namespace Magpie
{
    template<class T>
    class SimplexMethodView : public UiView
    {
    private:
        SimplexMethod2<T> s2;
        ArtificialBasis<T> artificialBasis;
        MementoHistory<SimplexMethod2<T>> h;
        MementoHistory<SimplexResultIterative2<T>> hist_result;
        SimplexResultIterative2<T> result;
        palka::Vec2f scale;

        template<typename... Args>
        static void ShowText(const std::string& rt_fmt_str, Args&& ... args)
        {
            ImGui::Text(vformat(rt_fmt_str, fmt::make_format_args(args...)).c_str());
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

        void simplex_table_render()
        {
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Simplex table").x) / 2.f);
            ImGui::Text("Simplex table");
            constexpr int cell_height = 120;
            ImGuiTableFlags flags = ImGuiTableFlags_Borders;
            auto changed_row_color = ImVec4(0.9, 0.322, 0.375, 0.9);
            auto changed_col_color = ImVec4(0.2, 0.922, 0.375, 0.9);
            auto resolving_elem_color = ImVec4(0.4, 0.522, 0.775, 0.9);
            auto table_sz = ImVec2(cell_height * result.after.columns_count(), static_cast<float>(cell_height * (result.after.rows_count() + 1)));
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - table_sz.x) / 2.f);
            ImU32 cell_bg_color = ImGui::GetColorU32(ImGuiCol_TableHeaderBg);
            if(ImGui::BeginTable("Simple6", result.after.columns_count() + 1, flags, table_sz))
            {
                for(int i = 0; i < result.after.rows_count(); ++i)
                {
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                    if(i == 0)
                    {
                        ImGui::TableNextColumn();
                        ShowCellText(i, cell_height, fmt::format("X({})", result.arti_basis_iters));
                        for(int j = 0; j < result.after.columns_count() - 1; ++j)
                        {
                            ImGui::TableNextColumn();
                            layout2(result.vars[j], j, cell_height);
                            //layout2("x%s ", std::to_string(j), j, cell_height);
                            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                        }
                        ShowCellText(i, cell_height, "B");
                        ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                    }

                    if(i < result.after.rows_count() - 2)
                    {
                        ImGui::TableNextColumn();
                        layout2(result.basis[i], i, cell_height);
                        //layout2("x%s ", std::to_string(result.basis[i]), i, cell_height);
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                    } else
                    {
                        ImGui::TableNextColumn();
                        ShowCellText(i, cell_height, "F");
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
                        ShowCellText(j, cell_height, result.after.get(j, i), " (?)");
                        if(ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
                        {
                            ImGui::BeginTooltip();
                            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
                            if(i == result.resolving_row && j == result.resolving_col)
                            {
                                std::string vf;
                                if(storage->type != DataStorage::FRACTUS)
                                    vf = "1 / {:.3f} = {:.3f} ";
                                else
                                    vf = "1 / {} = {} ";
                                ShowText(vf, result.before.get(result.resolving_col, result.resolving_row),
                                         result.after.get(j, i));
                            } else if(i == result.resolving_row)
                            {
                                std::string vf;
                                if(storage->type != DataStorage::FRACTUS)
                                    vf = "{:.3f} / {:.3f} = {:.3f}";
                                else
                                    vf = "{} / {} = {}";
                                ShowText(vf, result.before.get(i, result.resolving_row),
                                         result.before.get(result.resolving_col, result.resolving_row),
                                         result.after.get(i, result.resolving_row));
                            } else if(j == result.resolving_col)
                            {
                                std::string vf;
                                if(storage->type != DataStorage::FRACTUS)
                                    vf = "{:.3f} / {:.3f} * -1 = {:.3f}";
                                else
                                    vf = "{} / {} * -1 = {}";
                                ShowText(vf, result.before.get(result.resolving_col, i),
                                         result.before.get(result.resolving_col, result.resolving_row),
                                         result.after.get(result.resolving_col, i));
                            } else
                            {
                                std::string vf;
                                if(storage->type != DataStorage::FRACTUS)
                                    vf = "{:.3f} - {:.3f} * {:.3f} = {:.3f}";
                                else
                                    vf = "{} - {} * {} = {} ";
                                ShowText(vf, result.before.get(j, i),
                                         result.after.get(j, result.resolving_row),
                                         result.before.get(result.resolving_col, i),
                                         result.after.get(j, i));
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
            constexpr int cell_height = 120;
            constexpr int cell_width = 120;
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Select support element").x) / 2.f);
            ImGui::Text("Select support element: ");
            ImGuiTableFlags flags = ImGuiTableFlags_Borders;
            static int selected = -1;
            auto table_sz = ImVec2(cell_width * result.after.columns_count(), static_cast<float>(cell_height * (result.after.rows_count() + 1)));
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - table_sz.x) / 2.f);
            ImU32 cell_bg_color = ImGui::GetColorU32(ImGuiCol_TableHeaderBg);
            if(ImGui::BeginTable("##Simple1", result.after.columns_count() + 1, flags, table_sz))
            {
                for(int i = 0; i < result.after.rows_count(); ++i)
                {
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                    if(i == 0)
                    {
                        ImGui::TableNextColumn();
                        ShowCellText(i, cell_height, fmt::format("X({})", result.arti_basis_iters));
                        for(int j = 0; j < result.after.columns_count() - 1; ++j)
                        {
                            ImGui::TableNextColumn();
                            layout2(result.vars[j], j, cell_height);
                            ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                        }
                        ImGui::TableNextColumn();
                        ShowCellText(i, cell_height, "B");
                        ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                    }

                    if(i < result.after.rows_count() - 1)
                    {
                        ImGui::TableNextColumn();
                        layout2(result.basis[i], i, cell_height);
                        ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                    } else
                    {
                        ImGui::TableNextColumn();
                        ShowCellText(i, cell_height, "F");
                    }

                    for(int j = 0; j < result.after.columns_count(); ++j)
                    {
                        ImGui::TableNextColumn();
                        if(result.possibleSupportElems.contains(j) && result.possibleSupportElems[j] == i)
                        {
                            if(layout_selectable(result.after.get(j, i), cell_height, cell_width))
                            {
                                selected = j;
                                s2.setSupportElem(i, j);
                            }
                            if(selected == j)
                            {
                                ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(0.17, 0.678, 0.125, 0.9));
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                            } else
                            {
                                ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(0.215, 0.368, 0.678, 0.9));
                                ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
                            }
                        } else
                        {
                            ShowCellText(i, cell_height, result.after.get(j, i));
                        }
                    }
                }
                ImGui::EndTable();
            }
        }

        void result_render()
        {
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize("Simplex method is completed").x) / 2.f);
            ImGui::Text("Simplex method is completed");
            ImGui::Spacing();
            ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
            constexpr int cell_height = 120;
            auto table_sz = ImVec2(cell_height * result.after.columns_count(), static_cast<float>(cell_height * result.after.rows_count()));
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - table_sz.x) / 2.f);
            if(ImGui::BeginTable("Simple2", result.after.columns_count(), flags, table_sz))
            {
                int id = 0;
                for(int i = 0; i < result.after.rows_count(); ++i)
                {
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                    for(int j = 0; j < result.after.columns_count(); ++j)
                    {
                        ImGui::TableNextColumn();
                        ImGui::PushID(id++);
                        ShowCellText(j, cell_height, result.after.get(j, i));
                        //layout2("%s", result.after.get(j, i), j, cell_height);
                        ImGui::PopID();
                    }
                }
                ImGui::EndTable();
            }
            ImGui::Spacing();
            std::vector<double> resultVector;
            resultVector.resize(result.vars.size() + result.basis.size());
            int i = 0;
            for(auto& b: result.basis)
            {
                resultVector[b - 1] = (double) result.after.get(result.after.columns_count() - 1, i);
                i++;
            }
            for(auto& b: result.vars)
                resultVector[b - 1] = 0;

            std::string buildder = "X* {";
            std::string vf;
            if(storage->type != DataStorage::FRACTUS)
                vf = "{:.3f} ";
            else
                vf = "{} ";
            for(auto& v: resultVector)
                buildder += vformat(vf, fmt::make_format_args(v));
            buildder += "}";
            ImGui::Text(buildder.c_str());
            double res;
            if(storage->ptype == DataStorage::Minimization)
                res = (double)-result.after.get(result.after.columns_count() - 1, result.after.rows_count() - 1);
            else
                res =  (double)result.after.get(result.after.columns_count() - 1, result.after.rows_count() - 1);
            ImGui::Text(fmt::format("Result: {}", res).c_str());
        }

        void table_render()
        {
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - ImGui::CalcTextSize(result.answer_str.c_str()).x) / 2.f);
            ImGui::Text(result.answer_str.c_str());
            ImGui::Spacing();
            ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
            constexpr int cell_height = 120;
            auto table_sz = ImVec2(cell_height * result.after.columns_count(), static_cast<float>(cell_height * result.after.rows_count()));
            ImGui::SetCursorPosX((ImGui::GetWindowWidth() - table_sz.x) / 2.f);
            if(ImGui::BeginTable("Simple3", result.after.columns_count(), flags, table_sz))
            {
                int id = 0;
                for(int i = 0; i < result.after.rows_count(); ++i)
                {
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                    for(int j = 0; j < result.after.columns_count(); ++j)
                    {
                        ImGui::TableNextColumn();
                        ImGui::PushID(id++);
                        ShowCellText(j, cell_height, result.after.get(j, i));
                        ImGui::PopID();
                    }
                }
                ImGui::EndTable();
            }
        }

        void input_r()
        {
            ImGui::SetCursorPosY(40);
            constexpr int cell_height = 110;
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
            if(ImGui::BeginTable("##Simple4", result.after.columns_count(), flags, table_sz))
            {
                int id = 0;
                for(int row = 1; row < result.after.rows_count(); row++)
                {
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                    for(int column = 0; column < result.after.columns_count() - 2; column++)
                    {
                        ImGui::TableNextColumn();
                        ImGui::PushID(id++);
                        ShowCellText(column, cell_height, result.after.get(column, row));
                        // layout2("%s", result.after.get(column, row), column, cell_height);
                        ImGui::PopID();
                    }
                    ImGui::TableNextColumn();
                    ImGui::PushID(id++);
                    layout_sign((int) result.after.get(result.after.columns_count() - 2, row), 0, cell_height);
                    ImGui::TableNextColumn();
                    ShowCellText(4, cell_height, result.after.get(result.after.columns_count() - 1, row));
                    //layout2("%s", result.after.get(result.after.columns_count() - 1, row), 4, cell_height);
                    ImGui::PopID();
                }

                ImGui::EndTable();
            }
        }

        template<typename Argt>
        requires std::is_floating_point_v<Argt>
        static void ShowCellText(int col, int col_h, const Argt& arg, std::string add = "")
        {
            auto str = vformat("{:.3f}" + add, fmt::make_format_args(arg));
            auto size = ImGui::CalcTextSize(str.c_str());
            float ww = ImGui::GetColumnWidth(col);
            auto start_pos = ImGui::GetCursorPos();
            ImGui::SetCursorPosY(start_pos.y + (col_h - size.y) / 2.f);
            ImGui::SetCursorPosX(start_pos.x + (ww - size.x) / 2.f);
            ImGui::Text(str.c_str());
        }

        template<typename Argt>
        static void ShowCellText(int col, int col_h, const Argt& arg, std::string add = "")
        {
            auto str = vformat("{}" + add, fmt::make_format_args(arg));
            auto size = ImGui::CalcTextSize(str.c_str());
            float ww = ImGui::GetColumnWidth(col);
            auto start_pos = ImGui::GetCursorPos();
            ImGui::SetCursorPosY(start_pos.y + (col_h - size.y) / 2.f);
            ImGui::SetCursorPosX(start_pos.x + (ww - size.x) / 2.f);
            ImGui::Text(str.c_str());
        }

        static void ShowCellText(int col, int col_h, const char* str, std::string add = "")
        {
            //auto str = vformat("{}" + add, fmt::make_format_args(arg));
            auto size = ImGui::CalcTextSize(str);
            float ww = ImGui::GetColumnWidth(col);
            auto start_pos = ImGui::GetCursorPos();
            ImGui::SetCursorPosY(start_pos.y + (col_h - size.y) / 2.f);
            ImGui::SetCursorPosX(start_pos.x + (ww - size.x) / 2.f);
            ImGui::Text(str);
        }

        static void ShowCellText(int col, int col_h, const std::string& str, std::string add = "")
        {
            //auto str = vformat("{}" + add, fmt::make_format_args(arg));
            auto size = ImGui::CalcTextSize(str.c_str());
            float ww = ImGui::GetColumnWidth(col);
            auto start_pos = ImGui::GetCursorPos();
            ImGui::SetCursorPosY(start_pos.y + (col_h - size.y) / 2.f);
            ImGui::SetCursorPosX(start_pos.x + (ww - size.x) / 2.f);
            ImGui::Text(str.c_str());
        }

        template<typename Argt>
        void layout2(const Argt& value, int col, int col_h)
        {
            auto str = vformat("x{}", fmt::make_format_args(value));
            auto size = ImGui::CalcTextSize(str.c_str());
            float ww = ImGui::GetColumnWidth(col);
            auto start_pos = ImGui::GetCursorPos();
            ImGui::SetCursorPosY(start_pos.y + (col_h - size.y) / 2.f);
            ImGui::SetCursorPosX(start_pos.x + (ww - size.x) / 2.f);
            ImGui::Text(str.c_str());
        }

        void layout2(std::string_view fmt, std::string val, int col, int col_h)
        {
            auto size = ImGui::CalcTextSize(val.c_str());
            float ww = col_h;
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

        void layout2(std::string_view fmt, float val, int col, int col_h)
        {
            auto size = ImGui::CalcTextSize("0.002");
            float ww = ImGui::GetColumnWidth(col);
            auto start_pos = ImGui::GetCursorPos();
            ImGui::SetCursorPosY(start_pos.y + (col_h - size.y) / 2.f);
            ImGui::SetCursorPosX(start_pos.x + (ww - size.x) / 2.f);
            ImGui::Text("%.3f", val);
        }

        template<typename Argt>
        requires std::is_floating_point_v<Argt>
        bool layout_selectable(const Argt& value, int cell_height, int cell_width)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.2f, 0.5f));
            auto ret = ImGui::Selectable(vformat("{:.3f}", fmt::make_format_args(value)).c_str(), false, ImGuiItemFlags_None, ImVec2(cell_width, cell_height));
            ImGui::PopStyleVar();
            ImGui::PopStyleVar();
            return ret;
        }

        template<typename Argt>
        bool layout_selectable(const Argt& value, int cell_height, int cell_width)
        {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, ImVec2(0.2f, 0.5f));
            auto ret = ImGui::Selectable(vformat("{}", fmt::make_format_args(value)).c_str(), false, ImGuiItemFlags_None, ImVec2(cell_width, cell_height));
            ImGui::PopStyleVar();
            ImGui::PopStyleVar();
            return ret;
        }

        void layout_sign(int val, int col, int col_h)
        {
            auto ssign = signToStr((Sign) val);
            auto size = ImGui::CalcTextSize(ssign.data());
            float ww = ImGui::GetColumnWidth(col);
            auto start_pos = ImGui::GetCursorPos();
            ImGui::SetCursorPosY(start_pos.y + (col_h - size.y) / 2.f);
            ImGui::SetCursorPosX(start_pos.x + (ww - size.x) / 2.f);
            ImGui::Text(ssign.data());
        }

        void cellLayout(const char* label, size_t& val, int col)
        {
            constexpr auto cell_h = 80;
            constexpr auto input_w = 50;
            constexpr auto padding = 6;
            auto label_sz = ImGui::CalcTextSize(label);
            ImGui::AlignTextToFramePadding();
            const float full_w = input_w + label_sz.x + padding;
            float ww = ImGui::GetColumnWidth(col);
            const float center_offset = (ww - full_w) / 2.f;
            auto start_pos = ImGui::GetCursorPos();
            ImGui::SetCursorPosX(start_pos.x + center_offset);
            ImGui::SetCursorPosY(start_pos.y + cell_h / 2.f - label_sz.y);
            ImGui::TextUnformatted(label);
            ImGui::SameLine();

            ImGui::SetCursorPos(start_pos + ImVec2(center_offset + label_sz.x + padding, cell_h / 2.f - label_sz.y));
            auto pos_l = ImGui::GetCursorPos();
            auto w = ImGui::CalcTextSize("0.000").x;
            ImGui::PushItemWidth(w);
            ImGui::SetCursorPosX(pos_l.x + (input_w - w) / 2.f);
            ImGui::InputScalar(fmt::format("##{0}", label).c_str(), ImGuiDataType_::ImGuiDataType_U64, &val, 0);
            ImGui::PopItemWidth();
        }

        bool basisInput = true;
        std::vector<size_t> basis;
    public:
        SimplexMethodView(SimplexMethodView&&) = default;

        SimplexMethodView& operator=(SimplexMethodView&&) noexcept = default;

        void init() override
        {
            basis.resize(storage->data.rows_count() - 1);
        }

        SimplexMethodView(palka::Vec2f scale, bool open = true,
                          ImGuiWindowFlags w_flag = ImGuiWindowFlags_None)
                : UiView("SimplexMethodView", Config::WindowSize * scale, open, w_flag), scale(scale)
        {}

        ~SimplexMethodView() override = default;

        void render(palka::Window& w) override
        {
        }

        void inputBasis()
        {
            static bool valid = true;
            ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_NoSavedSettings;
            auto table_row_sz = ImVec2{(basis.size() - 1) * 150.f, 80};
            if(ImGui::BeginChild("##MainRegion", ImVec2(size.x, size.y * 0.86f), false,
                                 ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                                 ImGuiWindowFlags_NoBackground))
            {
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40.f);
                ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 85) / 2.f);
                ImGui::Text("Input basis");
                ImGui::SetCursorPosX((ImGui::GetWindowWidth() - table_row_sz.x) / 2.f);
                if(ImGui::BeginTable("Simple5", basis.size(), flags, table_row_sz))
                {
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, 80);
                    for(int column = 0; column < basis.size(); ++column)
                    {
                        ImGui::PushID(column);
                        ImGui::TableNextColumn();
                        std::string label = "b" + std::to_string(column);
                        cellLayout(label.c_str(), basis[column], column);
                        ImGui::PopID();
                    }
                    ImGui::EndTable();
                }
                ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 120.f) / 2.f);
                if(ImGui::Button("Next", {120, 36}))
                {
                    valid = true;
                    std::set<size_t> s(basis.begin(), basis.end());
                    if(s.size() != basis.size())
                        valid = false;
                    for(auto& b: s)
                        if(b < 0 || b > storage->data.columns_count())
                            valid = false;
                    if(valid)
                    {
                        basisInput = false;
                        result = s2.init(storage->getData<T>(), storage->ptype,basis);
                        h.update(s2);
                        hist_result.update(result);
                    }
                }
                if(!valid)
                {
                    ImVec4 color(255, 0, 0, 255);
                    ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 85) / 2.f);
                    ImGui::TextColored(color, "Invalid input");
                }
                ImGui::EndChild();
            }
        }

        void simplexIteration()
        {
            if(ImGui::BeginChild("##MainRegion", ImVec2(size.x - 40.f, size.y * 0.86f), false,
                                 ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                                 ImGuiWindowFlags_NoBackground))
            {
                if(result.siState == SimxpleMetState2::DONE_CANT_SOLVE)
                {
                    ImGui::Text("Can't solve");
                } else
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
                }
                ImGui::EndChild();
            }
            if(ImGui::BeginChild("##ControlRegion", {0, 45.f}, false, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoTitleBar |
                                                                      ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings |
                                                                      ImGuiWindowFlags_NoBackground))
            {
                ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 298.f) / 2.f);
                if(ImGui::Button("Previous iteration", {145, 35}))
                {
                    h.undo(s2);
                    hist_result.undo(result);
                }
                if(result.state != SiMetResultType::RESULT_TABLE && result.siState != SimxpleMetState2::DONE_CANT_SOLVE)
                {
                    ImGui::SameLine();
                    if(ImGui::Button("Next iteration", {145, 35}))
                    {
                        result = s2.solve_iterative();
                        hist_result.update(result);
                        h.update(s2);
                    }
                }
                ImGui::EndChild();
            }
        }

        void update() override
        {
            size = Config::WindowSize * scale;
            ImGui::SetNextWindowPos(ImVec2((Config::WindowSize.x - (size.x)) / 2,
                                           (Config::WindowSize.y - (size.y)) / 2), ImGuiCond_Always, {0, 0});

            if(ImGui::Begin(name.c_str(), &open, win_flag))
            {
                ImGui::SetWindowSize(ImVec2{size.x, size.y});
                if(basisInput)
                    inputBasis();
                else
                    simplexIteration();
                navLayout(true);
                ImGui::End();
            }
        }

        void setEvents() override
        {}

    };
}

#endif //MAGPIE_SIMPLEXMETHODVIEW_H
