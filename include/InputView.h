#ifndef MAGPIE_INPUTVIEW_H
#define MAGPIE_INPUTVIEW_H

#include "UiView.h"
#include "config.h"
#include "imgui_internal.h"
#include "myImGui.h"
#include <implot.h>
#include <tracy/Tracy.hpp>
#include <nlohmann/json.hpp>

namespace Magpie
{
    class InputView : public UiView
    {
    private:
        int cols;
        int rows;
        bool isDone = false;
        bool inputFromJson = false;
        bool file_browser_op;
        MatrixStorage<UniversalInput> matrix;
        ImGui::FileManager_Context c;
        palka::Vec2f scale;
    public:
        InputView(std::string_view name, palka::Vec2f scale) : UiView(name, Config::WindowSize * scale), rows(0), cols(0), c("./", true, ".json"), scale(scale)
        {}

        void render(palka::Window& w) override
        {
            size = Config::WindowSize * scale;
            ImGui::SetNextWindowPos(ImVec2((Config::WindowSize.x - (size.x)) / 2,
                                           (Config::WindowSize.y - (size.y)) / 2), ImGuiCond_Always, {0, 0});
            if(ImGui::Begin(name.c_str()))
            {
                ImGui::SetWindowSize(ImVec2{size.x, size.y});
                ImGui::PushItemWidth(100);

                ImGui::Spacing();
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Enter the initial data.").x) / 2);
                ImGui::SetCursorPosY((ImGui::GetWindowSize().y - (ImGui::GetWindowSize().y / 2)) / 2);
                ImGui::Text("Enter the initial data.");
                ImGui::Spacing();
                ImGui::Spacing();

                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Number of variables (0<n<=5)").x - 101) / 2);
                if(ImGui::DragInt("Number of variables (1<n<=16)", &cols, 1, 1, 17))
                    inputFromJson = false;
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Number of variables (0<n<=5)").x - 101) / 2);
                if(ImGui::DragInt("Number of restrictions (1<n<=16)", &rows, 1, 1, 17))
                    inputFromJson = false;
                ImGui::Spacing();
                ImGui::Spacing();

                auto b_size3 = ImGui::CalcTextSize("Read from json").x + 20.f;
                ImGui::SetCursorPosX((ImGui::GetWindowSize().x - b_size3 - 15.f) / 2);
                file_browser_op = ImGui::Button("Read from json", {b_size3, 0});
                c.setOpen(file_browser_op);
                if(auto res = ImGui::FileManager(c); res.first)
                {
                    auto path = res.second;
                    std::ifstream f(path);
                    nlohmann::json json = nlohmann::json::parse(f);
                    auto& inputLayout = json["Input"];
                    matrix.alloc_matrix(inputLayout["Limitations"].size() + 1, inputLayout["Limitations"].begin()->size());
                    int i = 0;
                    for(auto& v: inputLayout["TargetFunc"])
                    {
                        matrix.get(i++, 0) = UniversalInput(v.dump());
                    }
                    i = 0;
                    int j = 1;
                    for(auto& v: inputLayout["Limitations"])
                    {
                        for(auto& v2: v)
                            matrix.get(i++, j) = UniversalInput(v2.dump());
                        i = 0;
                        j++;
                    }
                    rows = matrix.rows_count() - 1;
                    cols = matrix.columns_count() - 2;
                    inputFromJson = true;
                }

                ImGui::Spacing();
                ImGui::Spacing();
                auto b_size = ImGui::CalcTextSize("Entering restriction (graphics 2d)").x + 20.f;
                auto b_size2 = ImGui::CalcTextSize("Go to the choice of the solution method").x + 20.f;
                if(!inputFromJson)
                {
                    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - b_size * 2 - 15.f) / 2);
                    isDone = cols > 1 && cols <= 16 && rows > 1 && rows <= 16;
                    if(!isDone)
                    {
                        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
                        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
                    }
                    if(ImGui::Button("Entering restriction (matrix)", {b_size, 0}))
                    {
                        nextSatet = States::InputRestriction;
                        storage->data.alloc_matrix(rows + 1, cols + 2);
                        write_signs();
                        sceneCallback();
                    }
                    if(!isDone)
                    {
                        ImGui::PopItemFlag();
                        ImGui::PopStyleVar();
                    }
                    ImGui::SameLine();
                    if(ImGui::Button("Entering restriction (graphics 2d)", {b_size, 0}))
                    {
                        nextSatet = States::InputRestrictionGraph;
                        sceneCallback();
                    }
                } else
                {
                    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - b_size2 * 3 - 15.f) / 2);
                    if(ImGui::Button("Go to editing the matrix", {b_size2, 0}))
                    {
                        nextSatet = States::InputRestriction;
                        storage->data = matrix;
                        sceneCallback();
                    }
                    ImGui::SameLine();
                    if(ImGui::Button("Go to the choice of the solution method", {b_size2, 0}))
                    {
                        nextSatet = States::Menu;
                        storage->data = matrix;
                        sceneCallback();
                    }
                    ImGui::SameLine();
                    if(ImGui::Button("Delete Data", {b_size2, 0}))
                    {
                        inputFromJson = false;
                    }
                }
                ImGui::PopItemWidth();
                ImGui::End();
            }
        }

        void write_signs()
        {
            for(int i = 0; i < storage->data.rows_count(); ++i)
                storage->data.get(storage->data.columns_count() - 2, i) = UniversalInput((int) Sign::EQUAL);
        }

        std::pair<int, int> getResult()
        {
            return {rows, cols};
        }

        void update() override
        {}

        void setEvents() override
        {}
    };
}

#endif //MAGPIE_INPUTVIEW_H
