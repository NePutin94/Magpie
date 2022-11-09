#ifndef MAGPIE_SIMPLEXMETHODVIEW_H
#define MAGPIE_SIMPLEXMETHODVIEW_H

#include "UiView.h"
#include "SimplexMethod.h"
#include "SolverMemento.h"
#include "ArtificialBasis.h"
#include <imgui_internal.h>

namespace Magpie
{
    class SimplexMethodView : public UiView
    {
    private:
        //SimplexMethod<double> s;
        SimplexMethod2<double> s2;
        ArtificialBasis<double> artificialBasis;
        MementoHistory<SimplexMethod2<double>> h;
        MementoHistory<SimplexResultIterative2<double>> hist_result;
        SimplexResultIterative2<double> result;

        void simplex_table_render();

        void select_support_render();

        void result_render();

        void table_render();

        void history_render();

        void input_r();

        void layout2(std::string_view fmt, double val, int col, std::string pattern = "-0.00")
        {
            auto size = ImGui::CalcTextSize(pattern.c_str());
            float ww = ImGui::GetColumnWidth(col);
            auto start_pos = ImGui::GetCursorPos();
            ImGui::SetCursorPosY(start_pos.y + (60 - size.y) / 2.f);
            ImGui::SetCursorPosX(start_pos.x + (ww - size.x) / 2.f);
            ImGui::Text(fmt.data(), val);
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

        void layout_sign(const char* label, double val, int col)
        {
            //ImGui::AlignTextToFramePadding();
            auto start_pos = ImGui::GetCursorPos();
            auto size = ImGui::CalcTextSize("=");
            float input_w = size.x;
            float ww = ImGui::GetColumnWidth(col);
            const float center_offset = (ww - input_w) / 2.f;
            auto pos_s = ImGui::GetCursorScreenPos();
            ImGui::SetCursorPosX(start_pos.x + center_offset);
            ImGui::SetCursorPosY(start_pos.y + (60 - size.y) / 2.f);
            ImGui::Text(signToStr((Sign) val).data());
        }


    public:
        SimplexMethodView(SimplexMethodView&&) = default;

        SimplexMethodView& operator=(SimplexMethodView&&) = default;

        SimplexMethodView(palka::Vec2f size, MatrixStorage<double> store, std::vector<size_t> basis, bool open = true,
                          ImGuiWindowFlags w_flag = ImGuiWindowFlags_None)
                : UiView("SimplexMethodView", size, open, w_flag)
        {
            artificialBasis.init(store);
            artificialBasis.simplex_method();
            artificialBasis.simplex_method();
            artificialBasis.simplex_method();
            result = s2.init(store, basis);
            h.update(s2);
            hist_result.update(result);
        }

        ~SimplexMethodView() override = default;

        void render() override;

        void update() override;

        void setEvents() override;

    };
}

#endif //MAGPIE_SIMPLEXMETHODVIEW_H
