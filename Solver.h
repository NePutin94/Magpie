#ifndef BILLET_SOLVER_H
#define BILLET_SOLVER_H

#include <utility>
#include <vector>
#include <cassert>
#include <valarray>
#include <set>
#include "Plot.h"
#include "MatrixStorage.h"
#include "ISolver.h"

namespace Magpie
{
    template<class T>
    class GraphicsResult
    {
    public:
        enum Type
        {
            closed,
            open
        };
    private:
        std::vector<palka::Vec2<T>> Union;
        palka::Vec2<T> resultPoint;
        double resultValue;
        Type type;

    public:
        GraphicsResult(Type t, std::vector<palka::Vec2<T>> Union, const palka::Vec2<T>& resultPoint, double resultValue) : Union(std::move(Union)),
                                                                                                                           resultPoint(resultPoint),
                                                                                                                           resultValue(resultValue),
                                                                                                                           type(t)
        {

        }

        std::pair<std::vector<T>, std::vector<T>> getVisualUnion()
        {
            std::vector<T> X;
            std::vector<T> Y;
            for(auto vec: Union)
            {
                X.emplace_back(vec.x);
                Y.emplace_back(vec.y);
            }
            if(type == closed)
            {
                X.emplace_back(Union.front().x);
                Y.emplace_back(Union.front().y);
            }
            return {X, Y};
        }

        palka::Vec2<T> getResPoint()
        {
            return resultPoint;
        }

        double getResultValue()
        {
            return resultValue;
        }
    };

    class TestGetBazis
    {
    public:
        TestGetBazis()
        {}


        void init(int var_count, int limits)
        {
//            std::vector<std::pair<int, short>> new_vars; //saves the string in which the new variable will be added
//            for(int i = 1; i < limits; ++i)
//            {
//                if(auto s = (Sign) data.get(var_count - 1, i); s != Sign::EQUAL)
//                    new_vars.emplace_back(i, (s == Sign::LESSOREQUAL) ? 1 : -1);
//            }
//            MatrixStorage<double> kanonical_data(limits, var_count + new_vars.size());
//            for(int i = 0; i < limits; ++i)
//            {
//                int j = 0;
//                for(; j < var_count; ++j)
//                {
//                    kanonical_data.get(j, i);
//                }
//                for(auto& nv: new_vars)
//                {
//                    if(nv.first == i)
//                    {
//                        kanonical_data.get(++j, i) = 1;
//                        if(nv.second == -1)
//                        {
//                            for(int _i = 0; _i < var_count; ++_i)
//                                kanonical_data.get(_i, i) *= -1;
//                        }
//                    } else
//                        kanonical_data.get(++j, i) = 0;
//                }
//            }
//            data = kanonical_data;
        }

        auto solver(std::vector<std::vector<double>> matrix)
        {
            for(int col = 0; col < matrix.size() - 1; ++col)
            {
                double a = matrix[col][col];
                if(std::abs(a) != 1)
                {
                    for(double& j: matrix[col])
                        j = j / a;
                }

                for(int i = 0; i < matrix.size(); ++i)
                {
                    if(i == col) continue;
                    auto b = matrix[i][col];
                    for(int j = 0; j < matrix[0].size(); ++j)
                        matrix[i][j] -= b * matrix[col][j];
                }
            }

            bool isIdentity = true;
            for(int i = 0; i < matrix.size(); ++i)
            {
                for(int j = 0; j < matrix.size() - 1; ++j)
                {
                    if(i == j)
                    {
                        if(matrix[i][j] != 1)
                        {
                            isIdentity = false;
                            break;
                        }
                    } else
                    {
                        if(matrix[i][j] != 0)
                        {
                            isIdentity = false;
                            break;
                        }
                    }
                }
            }

            if(isIdentity)
            {
                std::vector<std::vector<double>> res;
                res.resize(matrix.size());
                for(int i = 0; i < matrix.size(); ++i)
                {
                    res[i].resize(4);
                    for(int j = 2, j2 = 0; j < matrix[0].size(); ++j)
                    {
                        res[i][j2++] = matrix[i][j];
                        if(j2 == 2)
                            res[i][j2++] = 1;
                    }
                }
                return std::make_pair(isIdentity, res);
            }

            return std::make_pair(isIdentity, matrix);
        }

        auto solver(MatrixStorage<double> matrix)
        {
            for(int col = 1; col < matrix.rows_count(); ++col)
            {
                int true_col = col - 1;
                double a = matrix.get(true_col, col);
                if(std::abs(a) != 1)
                {
                    for(int i = 0; i < matrix.columns_count(); ++i)
                    {
                        if(i == matrix.columns_count() - 2) continue;
                        matrix.get(i, col) /= a;
                    }
                }

                for(int i = 0; i < matrix.rows_count(); ++i)
                {
                    if(i == col) continue;
                    auto b = matrix.get(true_col, i);
                    for(int j = 0; j < matrix.columns_count(); ++j)
                    {
                        if(j == matrix.columns_count() - 2) continue;
                        matrix.get(j, i) -= b * matrix.get(j, col);
                    }
                }
            }

            bool isIdentity = true;
            for(int i = 0; i < matrix.rows_count(); ++i)
            {
                for(int j = 0; j < matrix.rows_count() - 1; ++j)
                {
                    if(i == j)
                    {
                        if(matrix.get(j, i) != 1)
                        {
                            isIdentity = false;
                            break;
                        }
                    } else
                    {
                        if(matrix.get(j, i) != 0)
                        {
                            isIdentity = false;
                            break;
                        }
                    }
                }
            }

            if(isIdentity)
            {
                MatrixStorage<double> res(matrix.rows_count(), 4);
                for(int i = 0; i < matrix.rows_count(); ++i)
                {
                    for(int j = matrix.rows_count() - 1, j2 = 0; j < matrix.columns_count(); ++j)
                    {
                        res.get(j2++, i) = matrix.get(j, i);
                    }
                }
                return res;
            }
            return MatrixStorage<double>(matrix);
        }
    };

    template<class T>
    class GraphicMet2D : Solver<T, GraphicsResult<T>>
    {
    private:
        template<class>
        friend
        class GraphPrivateWrapper;

        Magpie::Plot<T> F;
        std::vector<Magpie::Plot<T>> Plots;
        std::vector<palka::Vec2<T>> Union;

        static void sort(std::vector<palka::Vec2<T>>& points, std::vector<Magpie::Plot<T>> plots)
        {
            std::vector<std::pair<palka::Vec2<T>, palka::Vec2<T>>> pairs;
            for(int i = 0; i < points.size(); ++i)
            {
                auto p = points[i];
                for(int j = i + 1; j < points.size(); ++j)
                {
                    auto p2 = points[j];
                    bool find = false;
                    for(auto& val: plots)
                    {
                        if(val.hasPoint(p) && val.hasPoint(p2))
                        {
                            pairs.template emplace_back(p, p2);
                            std::swap(points[i + 1], points[j]);
                            find = true;
                            break;
                        }
                    }
                    if(find)
                    {
                        i++;
                        break;
                    }
                }
            }
            if(pairs.size() > 2)
            {
                //don't compare pairs.size and 0 because of possible holes (with points.size %2 != 0) will break the algorithm
                //btw this comparison is not required
                for(int i = 0; i < pairs.size() - 1; ++i)
                {
                    auto& p1 = pairs[i];
                    int j = i + 1;
                    auto& p2 = pairs[j];
                    bool once = false;
                    for(auto& val: plots)
                    {
                        if(val.hasPoint(p1.first) && val.hasPoint(p2.first))
                        {
                            once = true;
                            std::swap(p1.first, p1.second);
                            break;
                        }
                        if(val.hasPoint(p1.second) && val.hasPoint(p2.second))
                        {
                            once = true;
                            std::swap(p2.first, p2.second);
                            break;
                        }
                        if(val.hasPoint(p1.first) && val.hasPoint(p2.second) || val.hasPoint(p1.second) && val.hasPoint(p2.first))
                        {
                            once = true;
                            break;
                        }
                    }
                    if(!once)
                    {
                        for(int b = j + 1; b < pairs.size(); ++b)
                        {
                            auto& p3 = pairs[b];
                            bool once = false;
                            for(auto& val: plots)
                            {
                                if(val.hasPoint(p1.first) && val.hasPoint(p3.first))
                                {
                                    once = true;
                                    std::swap(p1.first, p1.second);
                                    break;
                                }
                                if(val.hasPoint(p1.second) && val.hasPoint(p3.second))
                                {
                                    once = true;
                                    std::swap(p3.first, p3.second);
                                    break;
                                }
                                if(val.hasPoint(p1.first) && val.hasPoint(p3.second) || val.hasPoint(p1.second) && val.hasPoint(p3.first))
                                {
                                    once = true;
                                    break;
                                }
                            }
                            if(once)
                            {
                                std::swap(p2, p3);
                                break;
                            }
                        }
                    }
                }
//                std::sort(pairs.begin(), pairs.end(), [&](auto& p1, auto& p2)
//                {
//                    for(auto& val: plots)
//                    {
//                        if(val.hasPoint(p1.first) && val.hasPoint(p2.first))
//                        {
//                            std::swap(p1.first, p1.second);
//                            return true;
//                        }
//                        if(val.hasPoint(p1.second) && val.hasPoint(p2.second))
//                        {
//                            std::swap(p2.first, p2.second);
//                            return true;
//                        }
//                        if(val.hasPoint(p1.first) && val.hasPoint(p2.second) || val.hasPoint(p1.second) && val.hasPoint(p2.first))
//                        {
//                            return true;
//                        }
//                    }
//                    return false;
//                });
            } else
            {
                auto& p1 = pairs[0];
                auto& p2 = pairs[1];

                for(auto& val: plots)
                {
                    if(val.hasPoint(p1.first) && val.hasPoint(p2.first))
                    {
                        std::swap(p1.first, p1.second);
                        break;
                    }
                    if(val.hasPoint(p1.second) && val.hasPoint(p2.second))
                    {
                        std::swap(p2.first, p2.second);
                        break;
                    }
                }
            }

            std::pair<int, int> para{-1, -1};
            if(points.size() % 2 != 0) //finding a hole to insert the last element
            {
                for(int i = 0; i < pairs.size(); ++i)
                {
                    auto& p = pairs[i];
                    bool once1 = false;
                    bool once2 = false;
                    for(int j = 0; j < pairs.size(); ++j)
                    {
                        if(j != i)
                        {
                            auto& p2 = pairs[j];
                            for(auto& val: plots)
                            {
                                once1 = once1 || val.hasPoint(p.first) && val.hasPoint(p2.first) || val.hasPoint(p.first) && val.hasPoint(p2.second);
                                once2 = once2 || val.hasPoint(p.second) && val.hasPoint(p2.first) || val.hasPoint(p.second) && val.hasPoint(p2.second);
                            }
                        }
                    }
                    if(!once1)
                    {
                        bool hasPlot = false;
                        for(auto& val: plots) //checking for sets with real holes
                        {
                            if(val.hasPoint(p.first) && val.hasPoint(points.back()))
                            {
                                hasPlot = true;
                                break;
                            }
                        }
                        if(hasPlot)
                        {
                            para.first = i;
                            para.second = 1;

                            break;
                        }
                    } else if(!once2)
                    {
                        bool hasPlot = false;
                        for(auto& val: plots) //checking for sets with real holes
                        {
                            if(val.hasPoint(p.second) && val.hasPoint(points.back()))
                            {
                                hasPlot = true;
                                break;
                            }
                        }
                        if(hasPlot)
                        {
                            para.first = i;
                            para.second = 2;
                            break;
                        }
                    }
                }
            }

            {
                int i = 0;
                for(auto& p: pairs)
                {
                    if(i == para.first)
                    {
                        if(para.second == 1)
                            points[i++] = points.back();
                        points[i++] = p.first;
                        points[i++] = p.second;
                        if(para.second == 2)
                            points[i++] = points.back();
                    } else
                    {
                        points[i++] = p.first;
                        points[i++] = p.second;
                    }
                }
            }
//            T min = std::numeric_limits<T>::max();
//            int index = -1;
//            for(int i = 0; i < points.size(); ++i)
//            {
//                if(points[i].x < min)
//                {
//                    index = i;
//                    min = points[i].x;
//                }
//            }
//            if(index > -1)
//                std::rotate(points.begin(),
//                            points.end() - (points.size() - index),
//                            points.end());
        }

    public:
        GraphicMet2D() = default;

        GraphicMet2D(const MatrixStorage<T>& data, int var_count, int limitations_count)
        {
            F = Magpie::Plot<T>{data.get(0, 0), data.get(1, 0), 0};
            for(int i = 1; i < limitations_count; ++i)
            {
                auto a = data.get(0, i);
                auto b = data.get(1, i);
                auto c = data.get(3, i);
                auto s = (int) data.get(2, i);
                Plots.emplace_back(Magpie::Plot<T>{a, b, c, (Sign) s});
            }
            Plots.emplace_back(Magpie::Plot<T>{1, 0, 0, Sign::GREATEROREQUAL});
            Plots.emplace_back(Magpie::Plot<T>{0, 1, 0, Sign::GREATEROREQUAL});
        }

        void init(const MatrixStorage<T>& data, int var_count, int limitations_count)
        {
            if(var_count > 2)
            {
                TestGetBazis test;
                auto res = test.solver(data.getMatrix());
                MatrixStorage<T> data2(data);
                data2.setMatrix(res.second);
                Plots.clear();
                F = Magpie::Plot<T>{data2.get(0, 0), data2.get(1, 0), 0};
                for(int i = 1; i < limitations_count + 1; ++i)
                {
                    auto a = data2.get(0, i);
                    auto b = data2.get(1, i);
                    auto c = data2.get(3, i);
                    auto s = (int) data2.get(2, i);
                    Plots.emplace_back(Magpie::Plot<T>{a, b, c, (Sign) s});
                }
                Plots.emplace_back(Magpie::Plot<T>{1, 0, 0, Sign::GREATEROREQUAL});
                Plots.emplace_back(Magpie::Plot<T>{0, 1, 0, Sign::GREATEROREQUAL});
                return;
            }
            Plots.clear();
            F = Magpie::Plot<T>{data.get(0, 0), data.get(1, 0), 0};
            for(int i = 1; i < limitations_count + 1; ++i)
            {
                auto a = data.get(0, i);
                auto b = data.get(1, i);
                auto c = data.get(3, i);
                auto s = (int) data.get(2, i);
                Plots.emplace_back(Magpie::Plot<T>{a, b, c, (Sign) s});
            }
            Plots.emplace_back(Magpie::Plot<T>{1, 0, 0, Sign::GREATEROREQUAL});
            Plots.emplace_back(Magpie::Plot<T>{0, 1, 0, Sign::GREATEROREQUAL});
        }

        //find all the intersection points
        auto getIntersectionPoints(const std::vector<Magpie::Plot<T>>& plots)
        {
            std::vector<palka::Vec2<T>> points;
            for(int i = 0; i < plots.size(); ++i)
            {
                for(int j = i + 1; j < plots.size(); ++j)
                {
                    auto& v = plots[i];
                    auto& v2 = plots[j];
                    if(v != v2)
                    {
                        auto in = v.intersection(v2);
                        if(in.x >= 0 && in.y >= 0 && in.x != std::numeric_limits<T>::min() && in.y != std::numeric_limits<T>::min())
                        {
                            points.emplace_back(in);
                        }
                    }
                }
            }
            return points;
        }

        //we delete lines that do not set critical points, but only pass through them
        //// (they have one point in common with the set that they do not set)
        //BUG:it may happen that generateAAB returns an intersection point that forms a set
        auto deleteBadPlots(const std::vector<Magpie::Plot<T>>& plots, const std::vector<palka::Vec2<T>>& points)
        {
            std::set<Plot<T>> filteredLines;
            auto deleteCheck = [&](const Magpie::Plot<T>& v, palka::Vec2<T> in)
            {
                bool all = true;
                for(auto& p: v.generateAAB(in, 0.2))
                {
                    for(auto& l: plots)
                    {
                        if(!l.containsPoint(p))
                        {
                            all = false;
                            break;
                        }
                    }
                    if(all)
                    {
                        filteredLines.template emplace(v);
                        break;
                    }
                    all = true;
                }
                return all;
            };
            for(int i = 0; i < plots.size(); ++i)
            {
                for(int j = i + 1; j < plots.size(); ++j)
                {
                    auto& v = plots[i];
                    auto& v2 = plots[j];
                    if(v != v2)
                    {
                        auto in = v.intersection(v2);
                        if(in.x >= 0 && in.y >= 0 && in.x != std::numeric_limits<T>::min() && in.y != std::numeric_limits<T>::min())
                        {
                            bool all = deleteCheck(v, in);
//                            for(auto& p: v.generateAAB(in, 0.2))
//                            {
//                                for(auto& l: plots)
//                                {
//                                    if(!l.containsPoint(p))
//                                    {
//                                        all = false;
//                                        break;
//                                    }
//                                }
//                                if(all)
//                                {
//                                    filteredLines.template emplace(v);
//                                    break;
//                                }
//                                all = true;
//                            }
                            bool all2 = deleteCheck(v2, in);
//                            for(auto& p: v2.generateAAB(in, 0.2))
//                            {
//                                if(std::find_if(points.begin(), points.end(), [&](const auto& point)
//                                { return p == point; }) != points.end())
//                                    continue;
//
//                                for(auto& l: plots)
//                                {
//                                    if(!l.containsPoint(p))
//                                    {
//                                        all2 = false;
//                                        break;
//                                    }
//                                }
//                                if(all2)
//                                {
//                                    filteredLines.template emplace(v2);
//                                    break;
//                                }
//                                all2 = true;
//                            }
                        }
                    }
                }
            }
            return filteredLines;
        }

        GraphicsResult<T> solve() override
        {
            std::vector<palka::Vec2<T>> points = getIntersectionPoints(Plots);
//            for(int i = 0; i < Plots.size(); ++i) //find all the intersection points
//            {
//                for(int j = i + 1; j < Plots.size(); ++j)
//                {
//                    auto& v = Plots[i];
//                    auto& v2 = Plots[j];
//                    if(v != v2)
//                    {
//                        auto in = v.intersection(v2);//intersection({v.a, v2.a}, {v.b, v2.b}, {v.c, v2.c});
//                        if(in.x >= 0 && in.y >= 0 && in.x != std::numeric_limits<T>::min() && in.y != std::numeric_limits<T>::min())
//                        {
//                            points.emplace_back(in);
//                        }
//                    }
//                }
//            }

            std::set<Plot<T>> filteredLines = deleteBadPlots(Plots, points);
            //we delete lines that do not set critical points, but only pass through them
            //// (they have one point in common with the set that they do not set)
            //BUG:it may happen that generateAAB returns an intersection point that forms a set
//            for(int i = 0; i < Plots.size(); ++i)
//            {
//                for(int j = i + 1; j < Plots.size(); ++j)
//                {
//                    auto& v = Plots[i];
//                    auto& v2 = Plots[j];
//                    if(v != v2)
//                    {
//                        auto in = v.intersection(v2);//intersection({v.a, v2.a}, {v.b, v2.b}, {v.c, v2.c});
//                        if(in.x >= 0 && in.y >= 0 && in.x != std::numeric_limits<T>::min() && in.y != std::numeric_limits<T>::min())
//                        {
//                            bool all = true;
//                            for(auto& p: v.generateAAB(in, 0.2))
//                            {
//                                for(auto& l: Plots)
//                                {
//                                    if(!l.containsPoint(p))
//                                    {
//                                        all = false;
//                                        break;
//                                    }
//                                }
//                                if(all)
//                                {
//                                    filteredLines.template emplace(v);
//                                    break;
//                                }
//                                all = true;
//                            }
//                            bool all2 = true;
//                            for(auto& p: v2.generateAAB(in, 0.2))
//                            {
//                                if(std::find_if(points.begin(), points.end(), [&](const auto& point)
//                                { return p == point; }) != points.end())
//                                    continue;
//
//                                for(auto& l: Plots)
//                                {
//                                    if(!l.containsPoint(p))
//                                    {
//                                        all2 = false;
//                                        break;
//                                    }
//                                }
//                                if(all2)
//                                {
//                                    filteredLines.template emplace(v2);
//                                    break;
//                                }
//                                all2 = true;
//                            }
//                        }
//                    }
//                }
//            }
            Plots.clear();
            std::move(filteredLines.begin(), filteredLines.end(), std::back_inserter(Plots));

            for(auto& p: points) //discard those that do not lie in the domain of each function
            {
                bool all = true;
                for(auto& val: Plots)
                {
                    if(!val.containsPoint(p))
                    {
                        all = false;
                        break;
                    }
                }
                if(all)
                {
                    if(std::find_if(Union.begin(), Union.end(), [&](const palka::Vec2<T> r)
                    { return r == palka::Vec2<T>{abs(p.x), abs(p.y)}; })
                       == Union.end())
                    {
                        Union.emplace_back(abs(p.x), abs(p.y));
                    }
                }
            }


            GraphicMet2D<T>::sort(Union, Plots);

            struct holes_store
            {
                palka::Vec2<T> p;
                std::vector<Plot<T>> plots;
            };

            std::vector<holes_store> holes;
            if(Union.size() == 2)
            {
                std::vector<Plot<T>> plots1;
                std::vector<Plot<T>> plots2;
                bool once = false;
                for(auto& l: Plots)
                {
                    auto lp1 = l.hasPoint(Union[0]);
                    auto lp2 = l.hasPoint(Union[1]);
                    if(lp1 && lp2)
                    {
                        once = true;
                    }
                    if(lp1)
                        plots1.emplace_back(l);
                    if(lp2)
                        plots2.emplace_back(l);
                }
                holes.emplace_back(holes_store{Union[0], plots1});
                holes.emplace_back(holes_store{Union[1], plots2});
            } else
            {
                for(int i = 0; i < Union.size(); ++i)
                {
                    auto p1 = Union[i];
                    int j = (i + 1 < Union.size()) ? i + 1 : 0;
                    auto p2 = Union[j];
                    if(p1 == p2) continue;
                    std::vector<Plot<T>> plots1;
                    std::vector<Plot<T>> plots2;
                    bool once = false;
                    for(auto& l: Plots)
                    {
                        auto lp1 = l.hasPoint(p1);
                        auto lp2 = l.hasPoint(p2);
                        if(lp1 && lp2)
                            once = true;
                        if(lp1)
                            plots1.emplace_back(l);
                        if(lp2)
                            plots2.emplace_back(l);
                    }
                    if(!once)
                    {
                        holes.emplace_back(holes_store{p1, plots1});
                        holes.emplace_back(holes_store{p2, plots2});
                    }
                }
            }

            if(!holes.empty())
            {
                std::vector<std::pair<Plot<T>, palka::Vec2<T>>> pos;
                for(auto& h: holes)
                {
                    Plot<T> plot;
                    for(auto& p: h.plots)
                    {
                        bool hasone = false;
                        for(auto& up: Union)
                        {
                            if(up != h.p && p.hasPoint(up))
                            {
                                hasone = true;
                            }
                        }
                        if(!hasone)
                        {
                            plot = p;
                            break;
                        }
                    }

                    auto ps = plot.generateAAB(h.p);
                    for(auto& p: ps)
                    {
                        bool all = true;
                        for(auto& val: Plots)
                        {
                            if(!val.containsPoint(p))
                            {
                                all = false;
                                break;
                            }
                        }
                        if(all)
                        {
                            pos.emplace_back(plot, palka::Vec2<T>{p.x * 10, plot.getValueAtY(p.x * 10)});
                            break;
                        }
                    }
                }
                //it only works for two holes!!
                if(pos[0].first.hasPoint(Union.front()))
                {
                    Union.insert(Union.begin(), pos[0].second);
                    Union.insert(Union.end(), pos[1].second);
                } else
                {
                    Union.insert(Union.begin(), pos[1].second);
                    Union.insert(Union.end(), pos[0].second);
                }

                return {GraphicsResult<T>::open, Union, palka::Vec2<T>{}, 0};
            }

            float min = FLT_MAX;
            palka::Vec2<T> resVec;
            for(auto vec: Union) //find min point and value
            {
                if(auto val = F.a * vec.x + F.a * vec.y; val < min)
                {
                    min = F.a * vec.x + F.a * vec.y;
                    resVec = vec;
                }
            }
            double resValue = min;

            return {GraphicsResult<T>::closed, Union, resVec, resValue};
        }
    };

#if TESTING

    template<class T>
    class GraphPrivateWrapper
    {
    public:
        auto call(std::vector<palka::Vec2<T>> Union, std::vector<Magpie::Plot<T>> plots)
        {
            Magpie::GraphicMet2D<T>::sort(Union, plots);
            return Union;
        }
    };

#endif
}

#endif //BILLET_SOLVER_H
