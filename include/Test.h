#ifndef BILLET_TEST_H
#define BILLET_TEST_H

#include "UiView.h"
#include <implot.h>
#include <fmt/format.h>
#include "Solver.h"
#include "config.h"
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <imgui.h>

namespace Magpie
{
    class UniversalInput
    {
        std::string val;
    public:
        UniversalInput() = default;

        template<class T>
        UniversalInput(const T& value)
        {
            val = std::to_string(value);
        }

        std::string getValueNotNull()
        {
            if(val.empty())
                return "0";
            return val;
        }

        std::string& getValue()
        {
            return val;
        }
    };

    class Test : public UiView
    {
    private:
        MatrixStorage<UniversalInput> storage;

        void layout2(const char* label, double& val, int col)
        {
            auto label_sz = ImGui::CalcTextSize("value");
            auto input_sz = ImGui::CalcTextSize("-00.0000").x;

            ImGui::AlignTextToFramePadding();
            constexpr float padding = 2;
            const float full_w = input_sz + label_sz.x + padding;
            float ww = ImGui::GetColumnWidth(col);
            const float center_offset = (ww - full_w) / 2.f;
            auto start_pos = ImGui::GetCursorPos();
            ImGui::SetCursorPosX(start_pos.x + center_offset);
            ImGui::SetCursorPosY(start_pos.y + (58 - label_sz.y) / 2.f);
            ImGui::TextUnformatted(label);
            ImGui::SameLine();

            auto pos_l = ImGui::GetCursorPos();
            ImGui::PushItemWidth(input_sz);
            ImGui::SetCursorPosX(pos_l.x + padding);
            ImGui::InputDouble(fmt::format("##{0}", label).c_str(), &val, 0, 0, "%.3f");
            ImGui::PopItemWidth();
        }

        void layout_universal(const char* label, UniversalInput& val, int col)
        {
            auto label_sz = ImGui::CalcTextSize("value");
            auto input_sz = ImGui::CalcTextSize("-00.0000").x;

            ImGui::AlignTextToFramePadding();
            constexpr float padding = 2;
            const float full_w = input_sz + label_sz.x + padding;
            float ww = ImGui::GetColumnWidth(col);
            const float center_offset = (ww - full_w) / 2.f;
            auto start_pos = ImGui::GetCursorPos();
            ImGui::SetCursorPosX(start_pos.x + center_offset);
            ImGui::SetCursorPosY(start_pos.y + (58 - label_sz.y) / 2.f);
            ImGui::TextUnformatted(label);
            ImGui::SameLine();

            auto pos_l = ImGui::GetCursorPos();
            ImGui::PushItemWidth(input_sz);
            ImGui::SetCursorPosX(pos_l.x + padding);
            ImGui::InputText(fmt::format("##{0}", label).c_str(), &val.getValue());
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
            ImGui::SetCursorPosY(start_pos.y + (58 - label_sz.y) / 2.f); //12 is magic value for width 30
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

        bool layout_sign_universal(const char* label, UniversalInput& val, int col)
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
            ImGui::SetCursorPosY(start_pos.y + (58 - label_sz.y) / 2.f); //12 is magic value for width 30
            if(ImGui::Button(signToStr((Sign) std::stoi(val.getValue())).data(), {70, 0}))
                ImGui::OpenPopup("sign_popup");
            if(ImGui::BeginPopup("sign_popup"))
            {
                for(int i = -1; i < 2; i++)
                    if(ImGui::Selectable(signToStr((Sign) i).data()))
                    {
                        val.getValue() = std::to_string(i);
                        value_changed = true;
                    }
                ImGui::EndPopup();
            }
            return value_changed;
        }

        bool what(const char* label, int& numerator, int& denominator, int col)
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
            ImGui::SetCursorPosY(start_pos.y + 19 - label_sz.y / 2.f);
            ImGui::TextUnformatted("value");
            ImGui::SameLine();

            ImGui::SetCursorPos(start_pos + ImVec2(center_offset + label_sz.x + padding, 0));
            auto pos_s = ImGui::GetCursorScreenPos();
            auto pos_l = ImGui::GetCursorPos();
            //ImGui::RenderFrame(pos_s + ImVec2{0, 0}, pos_s + ImVec2{50, 46}, ImColor(29, 46, 72), false, 0.f);
            //ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
            auto w = ImGui::CalcTextSize("00000").x;
            ImGui::PushItemWidth(w);
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::SetCursorPosX(pos_l.x + (50 - w) / 2.f);
            ImGui::InputInt("##numerator", &numerator, 0, 0);
            p.y = ImGui::GetCursorScreenPos().y;
            ImGui::GetWindowDrawList()->AddLine(p, p + ImVec2(50, 0), IM_COL32(255, 255, 255, 255));
            ImGui::SetCursorPosX(pos_l.x + (50 - w) / 2.f);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 4);
            ImGui::InputInt("##denominator", &denominator, 0, 0);
#if 0
            ImGui::GetForegroundDrawList()->AddRect(ImVec2{center_offset, 0} + start_pos_l,
                                                    ImVec2{center_offset + full_w, 46} + start_pos_l,
                                                    IM_COL32(0, 255, 255, 255));
#endif
            //ImGui::PopStyleColor();
        }

#if DEBUG

        void fill(int var)
        {
            switch(var)
            {
                case 0:
                    storage.alloc_matrix(3, 6);
                    storage.get(0, 0) = -2;
                    storage.get(1, 0) = -1;
                    storage.get(2, 0) = -3;
                    storage.get(3, 0) = -1;

                    storage.get(0, 1) = 1;
                    storage.get(1, 1) = 2;
                    storage.get(2, 1) = 5;
                    storage.get(3, 1) = -1;
                    storage.get(4, 1) = 0;
                    storage.get(5, 1) = 4;

                    storage.get(0, 2) = 1;
                    storage.get(1, 2) = -1;
                    storage.get(2, 2) = -1;
                    storage.get(3, 2) = 2;
                    storage.get(4, 2) = 0;
                    storage.get(5, 2) = 1;
                    break;
                case 1: //open
                    storage.alloc_matrix(4, 4);
                    storage.get(0, 0) = -3;
                    storage.get(1, 0) = -2;

                    storage.get(0, 1) = 1;
                    storage.get(1, 1) = 2;
                    storage.get(2, 1) = 1;
                    storage.get(3, 1) = 7;

                    storage.get(0, 2) = 2;
                    storage.get(1, 2) = 1;
                    storage.get(2, 2) = 1;
                    storage.get(3, 2) = 8;

                    storage.get(0, 3) = 0;
                    storage.get(1, 3) = 1;
                    storage.get(2, 3) = 1;
                    storage.get(3, 3) = 3;
                    break;
                case 2://open
                    storage.alloc_matrix(4, 4);
                    storage.get(0, 0) = 1;
                    storage.get(1, 0) = 2;

                    storage.get(0, 1) = 2;
                    storage.get(1, 1) = -1;
                    storage.get(2, 1) = -1;
                    storage.get(3, 1) = -1;

                    storage.get(0, 2) = 1;
                    storage.get(1, 2) = -2;
                    storage.get(2, 2) = 1;
                    storage.get(3, 2) = 0;

                    storage.get(0, 3) = 1;
                    storage.get(1, 3) = 1;
                    storage.get(2, 3) = -1;
                    storage.get(3, 3) = 1;
                    break;
                case 3:
                    storage.alloc_matrix(5, 4);
                    storage.get(0, 0) = -1;
                    storage.get(1, 0) = -2;

                    storage.get(0, 1) = 2;
                    storage.get(1, 1) = 1;
                    storage.get(2, 1) = 1;
                    storage.get(3, 1) = 5;

                    storage.get(0, 2) = 1;
                    storage.get(1, 2) = 2;
                    storage.get(2, 2) = 1;
                    storage.get(3, 2) = 8;

                    storage.get(0, 3) = 5;
                    storage.get(1, 3) = 1;
                    storage.get(2, 3) = 1;
                    storage.get(3, 3) = 9;

                    storage.get(0, 4) = 6;
                    storage.get(1, 4) = -1;
                    storage.get(2, 4) = 1;
                    storage.get(3, 4) = 9;
                    break;
                case 4:
                    storage.alloc_matrix(6, 4);
                    storage.get(0, 0) = -1;
                    storage.get(1, 0) = -2;

                    storage.get(0, 1) = 2;
                    storage.get(1, 1) = 1;
                    storage.get(2, 1) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 1) = 5;

                    storage.get(0, 2) = 1;
                    storage.get(1, 2) = 2;
                    storage.get(2, 2) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 2) = 8;

                    storage.get(0, 3) = 1;
                    storage.get(1, 3) = 1;
                    storage.get(2, 3) = (int) Sign::GREATEROREQUAL;
                    storage.get(3, 3) = 1;

                    storage.get(0, 4) = -15;
                    storage.get(1, 4) = 2;
                    storage.get(2, 4) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 4) = -8;

                    storage.get(0, 5) = -10;
                    storage.get(1, 5) = 5;
                    storage.get(2, 5) = (int) Sign::GREATEROREQUAL;
                    storage.get(3, 5) = -8;
                    break;
                case 5:
                    storage.alloc_matrix(6, 4);
                    storage.get(0, 0) = -1;
                    storage.get(1, 0) = -2;

                    storage.get(0, 1) = 4;
                    storage.get(1, 1) = -1;
                    storage.get(2, 1) = (int) Sign::GREATEROREQUAL;
                    storage.get(3, 1) = -2;

                    storage.get(0, 2) = 1;
                    storage.get(1, 2) = -2;
                    storage.get(2, 2) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 2) = 0;

                    storage.get(0, 3) = 1;
                    storage.get(1, 3) = 2;
                    storage.get(2, 3) = (int) Sign::GREATEROREQUAL;
                    storage.get(3, 3) = 1;

                    storage.get(0, 4) = -3;
                    storage.get(1, 4) = 4;
                    storage.get(2, 4) = (int) Sign::GREATEROREQUAL;
                    storage.get(3, 4) = -1;

                    storage.get(0, 5) = -3;
                    storage.get(1, 5) = 3;
                    storage.get(2, 5) = (int) Sign::GREATEROREQUAL;
                    storage.get(3, 5) = -3;
                    break;
                case 6:
                    storage.alloc_matrix(7, 4);
                    storage.get(0, 0) = -1;
                    storage.get(1, 0) = -2;

                    storage.get(0, 1) = 1;
                    storage.get(1, 1) = 2;
                    storage.get(2, 1) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 1) = 8;

                    storage.get(0, 2) = 1;
                    storage.get(1, 2) = 4;
                    storage.get(2, 2) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 2) = 14;

                    storage.get(0, 3) = 1;
                    storage.get(1, 3) = -2;
                    storage.get(2, 3) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 3) = 1;

                    storage.get(0, 4) = -2;
                    storage.get(1, 4) = 4;
                    storage.get(2, 4) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 4) = 12;

                    storage.get(0, 5) = 1;
                    storage.get(1, 5) = 1;
                    storage.get(2, 5) = (int) Sign::GREATEROREQUAL;
                    storage.get(3, 5) = 1;

                    storage.get(0, 6) = 8;
                    storage.get(1, 6) = 1;
                    storage.get(2, 6) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 6) = 30;
                    break;
                case 7://open
                    storage.alloc_matrix(4, 4);
                    storage.get(0, 0) = -1;
                    storage.get(1, 0) = -2;

                    storage.get(0, 1) = -1;
                    storage.get(1, 1) = 1;
                    storage.get(2, 1) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 1) = -2;

                    storage.get(0, 2) = 1;
                    storage.get(1, 2) = -10;
                    storage.get(2, 2) = (int) Sign::GREATEROREQUAL;
                    storage.get(3, 2) = -30;

                    storage.get(0, 3) = 1;
                    storage.get(1, 3) = 1;
                    storage.get(2, 3) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 3) = 12;
                    break;
                case 8://open
                    storage.alloc_matrix(4, 4);
                    storage.get(0, 0) = -1;
                    storage.get(1, 0) = -2;

                    storage.get(0, 1) = 1;
                    storage.get(1, 1) = 1;
                    storage.get(2, 1) = (int) Sign::GREATEROREQUAL;
                    storage.get(3, 1) = 1;

                    storage.get(0, 2) = 1;
                    storage.get(1, 2) = -1;
                    storage.get(2, 2) = (int) Sign::GREATEROREQUAL;
                    storage.get(3, 2) = -2;

                    storage.get(0, 3) = 0;
                    storage.get(1, 3) = 1;
                    storage.get(2, 3) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 3) = 3;
                case 9://open
                    storage.alloc_matrix(8, 4);
                    storage.get(0, 0) = -1;
                    storage.get(1, 0) = -2;

                    storage.get(0, 1) = 6;
                    storage.get(1, 1) = 1;
                    storage.get(2, 1) = (int) Sign::GREATEROREQUAL;
                    storage.get(3, 1) = 2;

                    storage.get(0, 2) = 0;
                    storage.get(1, 2) = 1;
                    storage.get(2, 2) = (int) Sign::GREATEROREQUAL;
                    storage.get(3, 2) = 0;

                    storage.get(0, 3) = 1;
                    storage.get(1, 3) = -4;
                    storage.get(2, 3) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 3) = 5;

                    storage.get(0, 4) = 1;
                    storage.get(1, 4) = -2;
                    storage.get(2, 4) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 4) = 6;

                    storage.get(0, 5) = 1;
                    storage.get(1, 5) = -1;
                    storage.get(2, 5) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 5) = 8;

                    storage.get(0, 6) = -2;
                    storage.get(1, 6) = 1;
                    storage.get(2, 6) = (int) Sign::GREATEROREQUAL;
                    storage.get(3, 6) = -26;

                    storage.get(0, 7) = -10;
                    storage.get(1, 7) = 1;
                    storage.get(2, 7) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 7) = 8;
                    break;
                case 10://open
                    storage.alloc_matrix(9, 4);
                    storage.get(0, 0) = 1;
                    storage.get(1, 0) = 2;

                    storage.get(0, 1) = 6;
                    storage.get(1, 1) = 1;
                    storage.get(2, 1) = (int) Sign::GREATEROREQUAL;
                    storage.get(3, 1) = 2;

                    storage.get(0, 2) = 0;
                    storage.get(1, 2) = 1;
                    storage.get(2, 2) = (int) Sign::GREATEROREQUAL;
                    storage.get(3, 2) = 0;

                    storage.get(0, 3) = 1;
                    storage.get(1, 3) = -4;
                    storage.get(2, 3) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 3) = 5;

                    storage.get(0, 4) = 1;
                    storage.get(1, 4) = -2;
                    storage.get(2, 4) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 4) = 6;

                    storage.get(0, 5) = 1;
                    storage.get(1, 5) = -1;
                    storage.get(2, 5) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 5) = 8;

                    storage.get(0, 6) = -2;
                    storage.get(1, 6) = 1;
                    storage.get(2, 6) = (int) Sign::GREATEROREQUAL;
                    storage.get(3, 6) = -26;

                    storage.get(0, 7) = 2;
                    storage.get(1, 7) = 1;
                    storage.get(2, 7) = (int) Sign::GREATEROREQUAL;
                    storage.get(3, 7) = 6;

                    storage.get(0, 8) = -9;
                    storage.get(1, 8) = 1;
                    storage.get(2, 8) = (int) Sign::LESSOREQUAL;
                    storage.get(3, 8) = 18;
                    break;
                case 11://3d
                    storage.alloc_matrix(2, 5);
                    storage.get(0, 0) = 1;
                    storage.get(1, 0) = 1;
                    storage.get(2, 0) = -1;

                    storage.get(0, 1) = 1;
                    storage.get(1, 1) = 1;
                    storage.get(2, 1) = 1;
                    storage.get(3, 1) = (int) Sign::LESSOREQUAL;
                    storage.get(4, 1) = 1;
                    break;
            }
        }

#endif
    public:
        Test(palka::Vec2f pos, palka::Vec2f size, int n, int m, bool open = true, ImGuiWindowFlags w_flag = ImGuiWindowFlags_None)
                : UiView("EnteringRestrictions", pos, size, open, w_flag), storage(n + 1, m + 2)
        {
#if DEBUG
            fill(11);
#endif
        }

        void render(palka::Window& w) override
        {
            ImGui::SetNextWindowPos(ImVec2((Config::WindowSize.x - (size.x)) / 2,
                                           (Config::WindowSize.y - (size.y)) / 2), ImGuiCond_Always, {0, 0});
            ImGui::Begin(name.c_str(), &open, win_flag);
            ImGui::SetWindowSize({size.x, size.y});

            ImGui::Text("f = c_1*x_1 + x_2*x_2 + ... + c_n*x_n");
            if(ImGui::BeginTable("##table1", storage.columns_count() - 2, ImGuiTableFlags_SizingStretchProp))
            {
                ImGui::TableNextRow();
                for(int column = 0; column < storage.columns_count() - 2; ++column)
                {
                    ImGui::PushID(column);
                    ImGui::TableNextColumn();
                    std::string label = "x" + std::to_string(column);
                    layout_universal(label.c_str(), storage.get(column, 0), column);
                    ImGui::PopID();
                }
                ImGui::EndTable();
            }
            constexpr int cell_height = 60; //stores the height of the table element,
            // this is a constant value, but it is different for different table elements
            const float textPaddingY = 10.f;
            auto curr_wnd = ImGui::GetCurrentWindow();
            auto contentRect = curr_wnd->ContentRegionRect;
            palka::Vec2f sz = {contentRect.GetWidth(), contentRect.GetHeight()};
            //  ImVec2 center = contentRect.GetCenter();
            ImVec2 center_local = contentRect.GetCenter() - curr_wnd->Pos;
            ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
            ImVec2 TextPos{center_local.x - ImGui::CalcTextSize("a_1*x_1 + a_2*x_2 + ... + a_n*x_n [>=,=,<=] B").x / 2.f,
                           ImGui::GetCursorPosY() + textPaddingY};
            ImGui::SetCursorPos(TextPos);
            ImGui::Text("a_1*x_1 + a_2*x_2 + ... + a_n*x_n [>=,=,<=] B");
            ImVec2 barrier = TextPos + ImVec2{0, 60.f};

            if(auto pos = center_local.y - cell_height * storage.rows_count() / 2.f; barrier.y < pos) //center, 3 -> row count
                ImGui::SetCursorPosY(center_local.y - cell_height * storage.rows_count() / 2.f);
            else
                ImGui::SetCursorPosY(barrier.y);

            int id = 0;
            if(ImGui::BeginTable("Layout", storage.columns_count(), flags, {}))
            {
                auto t = ImGui::GetCurrentTable();
                for(int row = 1; row < storage.rows_count(); row++)
                {
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                    for(int column = 0; column < storage.columns_count() - 2; column++)
                    {
                        ImGui::TableNextColumn();
                        static int c = 0;
                        ImGui::PushID(id++);
                        layout_universal("value", storage.get(column, row), column);
                        ImGui::PopID();
                    }
                    ImGui::TableNextColumn();
                    ImGui::PushID(id++);
                    layout_sign_universal("##sign", storage.get(storage.columns_count() - 2, row), 0);

                    ImGui::TableNextColumn();
                    layout_universal("asd", storage.get(storage.columns_count() - 1, row), 4);
                    ImGui::PopID();
                }

                ImGui::EndTable();
                ImGui::GetForegroundDrawList()->AddRect(t->OuterRect.Min, t->OuterRect.Max, IM_COL32(0, 255, 255, 255));
            }
            //      ImGui::PopStyleVar();
            if(ImGui::Button("next"))
                sceneCallback();
            //ImGui::GetForegroundDrawList()->AddRect(contentRect.Min, contentRect.Max, IM_COL32(255, 0, 255, 255));
            //ImGui::GetForegroundDrawList()->AddCircle(center, 4, IM_COL32(255, 255, 0, 255));

            ImGui::End();
        }

#if DEBUG

        void update() override
        {
            //storage.debug();
        }

#else
        void update() override
        {

        }
#endif

        auto getResult()
        {
            MatrixStorage<double> fractus;
            fractus.alloc_matrix(storage.rows_count(), storage.columns_count());
            for(int i = 0; i < fractus.rows_count(); ++i)
            {
                for(int j = 0; j < fractus.columns_count(); ++j)
                {
                    fractus.get(j, i) = std::stod(storage.get(j, i).getValueNotNull());
                }
            }
            return fractus;
        }

        void setEvents() override
        {

        }
    };
}

#endif //BILLET_TEST_H
