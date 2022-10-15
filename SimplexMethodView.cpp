#include "SimplexMethodView.h"

void Magpie::SimplexMethodView::render()
{
    s2.data.debug();
    s2.input.debug();

    ImGui::Begin("Debug targetFunc");
    for(auto& v: s2.target_function)
    {
        ImGui::Text("%f ", v);
        ImGui::SameLine();
    }
    ImGui::End();

  //  s.data.debug();
  //  s.input.debug();
//    ImGui::Begin("Debug orderCoefficients");
//    for(auto& v: s.orderCoefficients)
//        ImGui::Text("a %i b %i ", v.first, v.second);
//    ImGui::End();
//    ImGui::Begin("Debug targetFunc");
//    for(auto& v: s.target_function)
//    {
//        ImGui::Text("%f ", v);
//        ImGui::SameLine();
//    }
//    ImGui::End();
//
//    ImGui::Begin("Debug Hist");
//    int id = 0;
//    for(auto& hist: h.curr)
//    {
//        if(!hist) continue;
//        ImGui::PushID(id++);
//        if(ImGui::TreeNode(fmt::format("id {}", id).c_str()))
//        {
//            ImGui::Text(fmt::format("algo state: {}", SimxpleMetState2String(hist.get()->alg_state)).c_str());
//            ImGui::Spacing();
//            hist->data.debugChild();
//            ImGui::TreePop();
//        }
//        ImGui::PopID();
//    }
//    ImGui::End();
}

void Magpie::SimplexMethodView::update()
{
//    static bool next = true;
//    while(next)
//    {
//        h.update(s);
//        auto res = s.solve_iterative();
//        next = res.state != SimxpleMetState::DONE_HAS_RESULT && res.state != SimxpleMetState::WAIT_DATA;
//    }
}

void Magpie::SimplexMethodView::setEvents()
{

}
