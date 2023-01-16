#ifndef BILLET_TEST_H
#define BILLET_TEST_H

#include "UiView.h"
#include <implot.h>
#include <fmt/format.h>
#include "GraphicsMethod2D.h"
#include "config.h"
#include <imgui_stdlib.h>
#include <imgui_internal.h>
#include <imgui.h>
#include "DataStorage.h"
#include "myImGui.h"

namespace Magpie
{
    class InputMatrix : public UiView
    {
    private:
        palka::Vec2f scale;
        bool file_browser_op = false;
        ImGui::FileManager_Context c;
        //MatrixStorage<UniversalInput> storage;

        void save(MatrixStorage<double> input, const std::string& path)
        {
            nlohmann::json json;
            auto& inputLayout = json["Input"];
            for(int i = 0; i < input.columns_count() - 1; ++i)
                inputLayout["TargetFunc"].emplace_back(input.get(i, 0));

            for(int i = 1; i < input.rows_count(); ++i)
            {
                auto& lim = inputLayout["Limitations"][fmt::format("Lim_{}", i)];
                for(int j = 0; j < input.columns_count(); ++j)
                    lim.emplace_back(input.get(j, i));
            }

            std::ofstream out(path);
            out << json.dump(4);
            out.close();
        }

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

        void cellLayout(const char* label, UniversalInput& val, int col)
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

        bool inputDone = false;

        void inputLayout()
        {
            auto curr_wnd = ImGui::GetCurrentWindow();
            auto contentRect = curr_wnd->ContentRegionRect;
            ImVec2 center_local = contentRect.GetCenter() - curr_wnd->Pos;
            static int item_current_idx = (int) storage->ptype;
            const char* combo_preview_value = storage->problemType[item_current_idx].data();
            ImGui::SetCursorPosX(center_local.x - 150 / 2.f);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 40);
            ImGui::PushItemWidth(150.f);
            if(ImGui::BeginCombo("Type", combo_preview_value))
            {
                for(int i = 0; i < storage->problemType.size(); i++)
                {
                    const bool is_selected = (item_current_idx == i);
                    if(ImGui::Selectable(storage->problemType[i].data(), is_selected))
                    {
                        inputDone = false;
                        item_current_idx = i;
                        storage->ptype = (DataStorage::ProblemType) item_current_idx;
                    }
                    if(is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::PopItemWidth();
            inputDataLayout3();
        }

        void inputDataLayout3()
        {
            inputDone = true;
            constexpr int padding = 30;
            constexpr int cell_height = 80; //stores the height of the table element,
            // this is a constant value, but it is different for different table elements
            ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_NoSavedSettings;
            int id = 0;
            ImGui::BeginChild("##ScrollingRegion", ImVec2(size.x - 20, size.y * 0.7f), true,
                              ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoTitleBar |
                              ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings);

            auto table_sz = ImVec2{storage->data.columns_count() * 150.f, static_cast<float>((storage->data.rows_count() - 1) * cell_height)};
            auto table_row_sz = ImVec2{(storage->data.columns_count() - 2) * 150.f, cell_height};
            auto pos = ImGui::GetCurrentWindow()->ContentRegionRect.GetCenter() - ImGui::GetCurrentWindow()->Pos;

            auto text1 = "f = c_1*x_1 + x_2*x_2 + ... + c_n*x_n";
            auto t1Size = ImGui::CalcTextSize(text1).x;
            auto text2 = "a_1*x_1 + a_2*x_2 + ... + a_n*x_n [>=,=,<=] B";
            auto t2Size = ImGui::CalcTextSize(text2).x;

            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
            ImGui::SetCursorPosX(pos.x - t1Size / 2.f);
            ImGui::Text("%s", text1);
            ImGui::Spacing();
            if(table_row_sz.x < size.x - 20)
                ImGui::SetCursorPosX(pos.x - table_row_sz.x / 2.f);
            if(ImGui::BeginTable("##Inputtargetfunc", storage->data.columns_count() - 2, flags, table_row_sz))
            {
                ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                for(int column = 0; column < storage->data.columns_count() - 2; ++column)
                {
                    ImGui::PushID(column);
                    ImGui::TableNextColumn();
                    std::string label = "x" + std::to_string(column);
                    cellLayout(label.c_str(), storage->data.get(column, 0), column);
                    ImGui::PopID();
                }
                ImGui::EndTable();
            }
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + padding);
            ImGui::SetCursorPosX(pos.x - t2Size / 2.f);
            ImGui::Text("%s", text2);
            ImGui::Spacing();
            if(table_sz.x < size.x - 20)
                ImGui::SetCursorPosX(pos.x - table_sz.x / 2.f);
            if(ImGui::BeginTable("##Inputlimit", storage->data.columns_count(), flags, table_sz))
            {
                for(int row = 1; row < storage->data.rows_count(); row++)
                {
                    ImGui::TableNextRow(ImGuiTableRowFlags_None, cell_height);
                    for(int column = 0; column < storage->data.columns_count() - 2; column++)
                    {
                        ImGui::TableNextColumn();
                        ImGui::PushID(id++);
                        cellLayout(fmt::format("a_{}_{}", row, column).c_str(), storage->data.get(column, row), column);
                        ImGui::PopID();
                    }
                    ImGui::TableNextColumn();
                    ImGui::PushID(id++);
                    layout_sign_universal("##sign", storage->data.get(storage->data.columns_count() - 2, row), 0);

                    ImGui::TableNextColumn();
                    cellLayout("B", storage->data.get(storage->data.columns_count() - 1, row), storage->data.columns_count() - 1);
                    ImGui::PopID();
                }
                ImGui::EndTable();
            }

            ImGui::EndChild();
        }

    public:

        InputMatrix(std::string_view name, palka::Vec2f scale)
                : UiView(name, Config::WindowSize * scale), scale(scale), c("./", true, ".json")
        {
            nextSatet = States::Menu;
        }

        void navLayout(bool isReady)
        {
            constexpr int b_padding = 40.f;
            ImVec2 b_size{80.f, 35.f};
            ImGui::Spacing();
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x * .5f);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - ((b_size.x * 2 + b_padding - 10) / 2.f) / 2.f);
            file_browser_op = ImGui::Button("Save", b_size);
            c.setOpen(file_browser_op);
            if(auto res = ImGui::FileManager(c); res.first)
            {
                auto matrix = storage->getData<double>();
                save(matrix, res.second);
            }
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
                sceneCallback();
            if(!isReady)
            {
                ImGui::PopItemFlag();
                ImGui::PopStyleVar();
            }
        }

        void render(palka::Window& w) override
        {
            size = Config::WindowSize * scale;
            ImGui::SetNextWindowPos(ImVec2((Config::WindowSize.x - (size.x)) / 2,
                                           (Config::WindowSize.y - (size.y)) / 2), ImGuiCond_Always, {0, 0});
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoResize;
            window_flags |= ImGuiWindowFlags_NoDecoration;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
            window_flags |= ImGuiWindowFlags_NoFocusOnAppearing;
            ImGui::Begin(name.c_str(), &open, window_flags);
            ImGui::SetWindowSize(ImVec2{size.x, size.y});
            inputLayout();
            navLayout(inputDone);
            ImGui::End();
        }

        void update() override
        {}

        auto getResult()
        {
            MatrixStorage<double> fractus;
            fractus.alloc_matrix(storage->data.rows_count(), storage->data.columns_count());
            for(int i = 0; i < fractus.rows_count(); ++i)
            {
                for(int j = 0; j < fractus.columns_count(); ++j)
                {
                    fractus.get(j, i) = std::stod(storage->data.get(j, i).getValueNotNull());
                }
            }
            return fractus;
        }

        void setEvents() override
        {}
    };
}

#endif //BILLET_TEST_H
