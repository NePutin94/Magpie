#ifndef BILLET_TEST_H
#define BILLET_TEST_H

#include "UiView.h"
#include <imgui_internal.h>
#include <implot.h>
#include <fmt/core.h>
#include "Solver.h"

namespace Magpie
{
    class Test : public UiView
    {
    private:
        MatrixStorage<double> storage;
        std::vector<Magpie::Plot<float>> testArr;
        palka::Vec2f result;
        std::vector<double> dataX;
        std::vector<double> dataY;

        GraphicMet2D<double> solver;

        void layout2(const char* label, double& val, int col)
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
            ImGui::SetCursorPosY(start_pos.y + 60 / 2.f - label_sz.y);
            ImGui::TextUnformatted("value");
            ImGui::SameLine();

            ImGui::SetCursorPos(start_pos + ImVec2(center_offset + label_sz.x + padding, 60 / 2.f - label_sz.y));
            auto pos_s = ImGui::GetCursorScreenPos();
            auto pos_l = ImGui::GetCursorPos();
            //ImGui::RenderFrame(pos_s + ImVec2{0, 0}, pos_s + ImVec2{50, 46}, ImColor(29, 46, 72), false, 0.f);
            //ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
            auto w = ImGui::CalcTextSize("00000").x;
            ImGui::PushItemWidth(w);
            ImVec2 p = ImGui::GetCursorScreenPos();
            ImGui::SetCursorPosX(pos_l.x + (50 - w) / 2.f);
            ImGui::InputDouble("##value", &val, 0, 0);
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
            ImGui::SetCursorPosY(start_pos.y + 60 / 2.f - label_sz.y); //12 is magic value for width 30
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
                case 1:
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
                case 2:
                    storage.alloc_matrix(4, 4);
                    storage.get(0, 0) = -1;
                    storage.get(1, 0) = -2;

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
                    storage.get(2, 1) = Sign::LESSOREQUAL;
                    storage.get(3, 1) = 5;

                    storage.get(0, 2) = 1;
                    storage.get(1, 2) = 2;
                    storage.get(2, 2) = Sign::LESSOREQUAL;
                    storage.get(3, 2) = 8;

                    storage.get(0, 3) = 1;
                    storage.get(1, 3) = 1;
                    storage.get(2, 3) = Sign::GREATEROREQUAL;
                    storage.get(3, 3) = 1;

                    storage.get(0, 4) = -15;
                    storage.get(1, 4) = 2;
                    storage.get(2, 4) = Sign::LESSOREQUAL;
                    storage.get(3, 4) = -8;

                    storage.get(0, 5) = -10;
                    storage.get(1, 5) = 5;
                    storage.get(2, 5) = Sign::GREATEROREQUAL;
                    storage.get(3, 5) = -8;
                    break;
                case 5:
                    storage.alloc_matrix(6, 4);
                    storage.get(0, 0) = -1;
                    storage.get(1, 0) = -2;

                    storage.get(0, 1) = 4;
                    storage.get(1, 1) = -1;
                    storage.get(2, 1) = Sign::GREATEROREQUAL;
                    storage.get(3, 1) = -2;

                    storage.get(0, 2) = 1;
                    storage.get(1, 2) = -2;
                    storage.get(2, 2) = Sign::LESSOREQUAL;
                    storage.get(3, 2) = 0;

                    storage.get(0, 3) = 1;
                    storage.get(1, 3) = 2;
                    storage.get(2, 3) = Sign::GREATEROREQUAL;
                    storage.get(3, 3) = 1;

                    storage.get(0, 4) = -3;
                    storage.get(1, 4) = 4;
                    storage.get(2, 4) = Sign::GREATEROREQUAL;
                    storage.get(3, 4) = -1;

                    storage.get(0, 5) = -3;
                    storage.get(1, 5) = 3;
                    storage.get(2, 5) = Sign::GREATEROREQUAL;
                    storage.get(3, 5) = -3;
                    break;
            }
        }

#endif
    public:
        Test(palka::Vec2f pos, palka::Vec2f size, int n, int m, bool open = true, ImGuiWindowFlags w_flag = ImGuiWindowFlags_None)
                : UiView("EnteringRestrictions", pos, size, open, w_flag), storage(n + 1, m + 2)
        {
            fill(5);
        }
//        using f_pair = std::pair<float, float>;
//
//        auto intersection(f_pair A, f_pair B, f_pair C)
//        {
//            f_pair out;
//            float det = A.first * B.second - A.second * B.first;
//            out.second = -(A.second * C.first - A.first * C.second) / det;
//            out.first = -(B.first * C.second - B.second * C.first) / det;
//            return out;
//        }
//
//        double det(double a, double b, double c, double d)
//        {
//            return a * d - b * c;
//        }
//
//        auto intersect(f_pair A, f_pair B, f_pair C)
//        {
//            float zn = det(A.first, A.second, B.first, B.second);
//            if(abs(zn) < 1e-9)
//                return f_pair{0, 0};
//            f_pair out;
//            out.first = -det(C.first, B.first, C.second, B.second) / zn;
//            out.second = -det(A.first, C.first, A.second, C.second) / zn;
//            return out;
//        }
//
//        palka::Vec2f Centroid(const std::vector<palka::Vec2f>& vertices, int vertexCount)
//        {
//            palka::Vec2f centroid = {0, 0};
//            double signedArea = 0.0;
//            double x0 = 0.0;
//            double y0 = 0.0;
//            double x1 = 0.0;
//            double y1 = 0.0;
//            double a = 0.0;
//
//            for(int i = 0; i < vertexCount; ++i)
//            {
//                x0 = vertices[i].x;
//                y0 = vertices[i].y;
//                x1 = vertices[(i + 1) % vertexCount].x;
//                y1 = vertices[(i + 1) % vertexCount].y;
//                a = x0 * y1 - x1 * y0;
//                signedArea += a;
//                centroid.x += (x0 + x1) * a;
//                centroid.y += (y0 + y1) * a;
//            }
//
//            signedArea *= 0.5;
//            centroid.x /= (6.0 * signedArea);
//            centroid.y /= (6.0 * signedArea);
//
//            return centroid;
//        }

        void OnTest()
        {
            solver.init(storage, 2, 4);
            auto res = solver.solve();
            auto u = res.getVisualUnion();
            dataX = u.first;
            dataY = u.second;
            result = res.getResPoint();

#if 0
            testArr.clear();

            for(int i = 1; i < 4; ++i)
            {
                auto a = (float) storage.get(0, i);
                auto b = (float) storage.get(1, i);
                auto c = (float) storage.get(3, i);
                auto s = storage.get(2, i);
                testArr.emplace_back(Billet::Plot2<float>{a, b, c, static_cast<int>(s)});
            }
            testArr.emplace_back(Billet::Plot2<float>{1, 0, 0, GREATEROREQUAL});
            testArr.emplace_back(Billet::Plot2<float>{0, 1, 0, GREATEROREQUAL});
//            std::sort(testArr.begin(), testArr.end(), [this](Billet::Plot2<float>& l, Billet::Plot2<float>& r)
//            {
//                auto in = intersection({l.a, r.a}, {l.b, r.b}, {r.c, l.c});
//                bool ret = false;
//                if(in.first >= 0 && in.second >= 0)
//                {
//                    for(float i = 0.01; i < 0.2; i += 0.01f)
//                    {
//                        in.first -= i;
//                        auto lv = l.getValueAtY(in.first);
//                        auto rv = r.getValueAtY(in.first);
//                        if(lv < rv)
//                            ret = true;
//                    }
//                } else
//                {
//                    for(float i = 0; i < 0.2; i += 0.01f)
//                    {
//
//                        auto lv = l.getValueAtY(i);
//                        auto rv = r.getValueAtY(i);
//                        if(lv < rv)
//                            ret = true;
//                    }
//                }
//                return ret;
//            });

            std::vector<palka::Vec2f> points;
            for(int i = 0; i < testArr.size(); ++i)
            {
                for(int j = i + 1; j < testArr.size(); ++j)
                {
                    auto& v = testArr[i];
                    auto& v2 = testArr[j];
                    if(v != v2)
                    {
                        auto in = intersection({v.a, v2.a}, {v.b, v2.b}, {v.c, v2.c});
                        if(in.first >= 0 && in.second >= 0)
                        {
                            points.emplace_back(palka::Vec2f{in.first, in.second});
                        }
                    }
                }
            }
            std::vector<palka::Vec2f> pointsready;
            for(auto& p: points)
            {
                bool all = true;
                for(auto& val: testArr)
                {
                    switch((_sign) val.sign)
                    {
                        case LESSOREQUAL:
                            if(!(val.a * p.x + val.b * p.y <= val.c))
                            {
                                all = false;
                                break;
                            }
                            break;
                        case GREATEROREQUAL:
                            if(!(val.a * p.x + val.b * p.y >= val.c))
                            {
                                all = false;
                                break;
                            }
                            break;
                        case EQUAL:
                            if(!(val.a * p.x + val.b * p.y == val.c))
                            {
                                all = false;
                                break;
                            }
                            break;
                    }

                }
                if(all)
                    pointsready.emplace_back(abs(p.x), abs(p.y));
            }


            std::sort(pointsready.begin(), pointsready.end(), [this](palka::Vec2f& l, palka::Vec2f& r)
            {
                return glm::length(l) < glm::length(r);
            });
            dataX.clear();
            dataY.clear();
            for(auto vec: pointsready)
            {
                dataX.emplace_back(vec.x);
                dataY.emplace_back(vec.y);
            }
            dataX.emplace_back(pointsready.front().x);
            dataY.emplace_back(pointsready.front().y);

            {
                center = Centroid(pointsready, pointsready.size());
                std::vector<palka::Vec2f> outP;
                auto a = (float) storage.get(0, 0);
                auto b = (float) storage.get(1, 0);
                palka::Vec2f antigrad{-a, -b};
                antigrad = glm::normalize(antigrad);
                int pos = 0;
                if(std::abs(a) * antigrad.x + std::abs(b) * antigrad.y < 0)
                {
                    pos = -1;
                } else
                {
                    pos = 1;
                }
                FilteredPX.clear();
                FilteredPY.clear();
                for(auto vec: pointsready)
                {
                    if(pos > 0 && center.x < vec.x && center.y < vec.y)
                    {
                        FilteredPX.emplace_back(vec.x);
                        FilteredPY.emplace_back(vec.y);
                        outP.emplace_back(vec);
                    } else if(pos < 0 && center.x > vec.x && center.y > vec.y)
                    {
                        FilteredPX.emplace_back(vec.x);
                        FilteredPY.emplace_back(vec.y);
                        outP.emplace_back(vec);
                    }
                }

                float min = FLT_MAX;

                for(auto vec: pointsready)
                {
                    if(auto val = a * vec.x + a * vec.y; val < min)
                    {
                        min = a * vec.x + a * vec.y;
                        result = vec;
                    }
                }

            }
#endif
//            for(int i = 0; i < pointsready.size(); i++)
//            {
//                auto vec = pointsready[i];
//                if(vec.x > 0 && vec.y > 0)
//                {
//                    if(i < pointsready.size() - 1)
//                    {
//                        auto next = pointsready[i + 1];
//                        auto inc = glm::normalize(next - vec);
//                        for(int i = 0; i < 100; ++i)
//                        {уж
//                            vec += inc;
//                            dataX.emplace_back(vec.x);
//                            dataY.emplace_back(vec.y);
//                            if(dataY.back() == 0) break;
//                            if(vec.x >= next.x || vec.y >= next.y)
//                            {
//                                dataX.back() = next.x;
//                                dataY.back() = next.y;
//                                break;
//                            }
//                        }
//                    } else
//                    {
//                        auto next = pointsready[0];
//                        auto inc = glm::normalize(next - vec);
//                        for(int i = 0; i < 100; ++i)
//                        {
//                            vec += inc;
//                            dataX.emplace_back(vec.x);
//                            dataY.emplace_back(vec.y);
//                            if(dataY.back() == 0) break;
//                            if(vec.x >= next.x || vec.y >= next.y)
//                            {
//                                dataX.back() = next.x;
//                                dataY.back() = next.y;
//                                break;
//                            }
//                        }
//                    }
//                }else
//                {
//
//                }
//            }

//            palka::Vec2f intersect;
//            for(int i = 0; i < testArr.size(); ++i)
//            {
//                std::vector<double> dataX;
//                std::vector<double> dataY;
//                auto& val = testArr[i];
//                bool currIntersect = false;
//                if(i < testArr.size() - 1)
//                {
//                    auto next = testArr[i + 1];
//                    auto in = intersection({val.a, next.a}, {val.b, next.b}, {val.c, next.c});
//                    if(in.first >= 0 && in.second >= 0)
//                    {
//                        currIntersect = true;
//                        intersect = {in.first, in.second};
//                    } else
//                        currIntersect = false;
//                }
//
//                for(int j = (prevIntersect) ? intersect.x : 0; j < 100; ++j)
//                {
//
//                    dataX.emplace_back((double) j);
//                    dataY.emplace_back((double) val.getValueAtY(j));
//                    if(dataY.back() == 0) break;
//                    if(currIntersect && palka::Vec2f{dataX.back(), dataY.back()} == intersect) break;
//                }
//
//                if(!currIntersect)
//                    prevIntersect = false;
//                else
//                    prevIntersect = true;
//
//                arrays.emplace_back(std::make_pair(dataX, dataY));
//            }

//
//            for(int i = 0; i < 100; ++i)
//            {
//                auto a = storage.get(0, 0);
//                auto b = storage.get(1, 0);
//                auto c = 0;
//                mainX[idex] = (double) i;
//                mainY[idex] = (double) (c - a * i) / (double) b;
//                idex++;
//            }
//            idex = 0;
//            f_pair inter;
//            {
//                auto a = storage.get(0, 1);
//                auto b = storage.get(1, 1);
//                auto c = storage.get(3, 1);
//                auto a1 = storage.get(0, 2);
//                auto b1 = storage.get(1, 2);
//                auto c1 = storage.get(3, 2);
//                inter = intersection({a, a1}, {b, b1}, {c, c1});
//            }
//            for(int i = 0; i < 100; ++i)
//            {
//                auto a = storage.get(0, 1);
//                auto b = storage.get(1, 1);
//                auto c = storage.get(3, 1);
//                dataX[idex] = (double) i;
//                dataY[idex] = (double) (c - a * i) / (double) b;
//                if(f_pair{dataX[idex], dataY[idex]} == inter) break;
//                if(dataY[idex] == 0) break;
//                idex++;
//            }
//            idex = 0;
//            for(int i = inter.first; i < 100; ++i)
//            {
//                auto a = storage.get(0, 2);
//                auto b = storage.get(1, 2);
//                auto c = storage.get(3, 2);
//                dataX2[idex] = (double) i;
//                dataY2[idex] = (double) (c - a * i) / (double) b;
//                if(dataY2[idex] == 0) break;
//                idex++;
//            }
        }

        void render() override
        {
            ImGui::Begin(name.c_str(), &open, win_flag);
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
            ImVec2 barrier = TextPos + ImVec2{0, 80.f};

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
                        //  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ((60 - t_sz.y) / 2.f));
                        //ImGui::TextUnformatted("value");
                        //ImGui::SameLine();
                        //float a;
                        //  ImGui::PushItemWidth(60);
                        //  auto prev = ImGui::GetCursorPos();
                        // ImVec2 p = ImGui::GetCursorScreenPos();
                        // ImGui::InputFloat("##text", &a);
                        static int c = 0;
                        ImGui::PushID(id++);
                        layout2("asd", storage.get(column, row), column);
                        ImGui::PopID();
//                        p.y = ImGui::GetCursorScreenPos().y;
//                        prev.y = ImGui::GetCursorPosY() + ImGui::GetCurrentContext()->Style.FramePadding.y + 2;
//                        ImGui::GetWindowDrawList()->AddLine(p, p + ImVec2(60, 0), IM_COL32(255, 255, 255, 255));
//                        ImGui::SetCursorPosX(prev.x);
//                        ImGui::InputFloat("##text", &a);
                    }
                    ImGui::TableNextColumn();
                    ImGui::PushID(id++);
                    layout_sign(signToStr((Sign) storage.get(storage.columns_count() - 2, row)).data(),
                                storage.get(storage.columns_count() - 2, row), 0);

                    ImGui::TableNextColumn();
                    layout2("asd", storage.get(storage.columns_count() - 1, row), 4);
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

//            ImGui::Begin("My Window");
//            if(ImPlot::BeginPlot("My Plot"))
//            {
//                ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
//                ImPlot::PlotLine("My Line Plot", dataX.data(), dataY.data(), dataX.size());
//                ImPlot::PlotScatter("Result", &result.x, &result.y, 1);
//                ImPlot::EndPlot();
//                ImPlot::PopStyleVar();
//            }
//            ImGui::End();
        }

#if DEBUG

        void update() override
        {
            storage.debug();
        }

#else
        void update() override
        {

        }
#endif

        auto getResult()
        {
            return storage;
        }

        void setEvents() override
        {

        }
    };
}

#endif //BILLET_TEST_H
