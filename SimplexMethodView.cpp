#include "SimplexMethodView.h"

void Magpie::SimplexMethodView::render()
{
    s.data.debug();
    s.input.debug();
    ImGui::Begin("Debug orderCoefficients");
    for(auto& v : s.orderCoefficients)
        ImGui::Text("a %i b %i ", v.first, v.second);
    ImGui::End();
    ImGui::Begin("Debug targetFunc");
    for(auto& v : s.target_function)
    {
        ImGui::Text("%f ", v);
        ImGui::SameLine();
    }
    ImGui::End();
}

void Magpie::SimplexMethodView::update()
{

}

void Magpie::SimplexMethodView::setEvents()
{

}
