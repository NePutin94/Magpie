#ifndef BILLET_SOLVER_H
#define BILLET_SOLVER_H

#include <utility>
#include <vector>
#include <cassert>
#include <valarray>
#include <set>
#include <ranges>
#include "Plot.h"
#include "MatrixStorage.h"
#include "ISolver.h"
#include <ConsoleLog.h>
#include <chrono>
#include <tracy/Tracy.hpp>
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

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
        enum Result
        {
            none,
            hasSolution,
            hasNoSolution
        };
    private:
        std::vector<palka::Vec2<T>> Union;
        palka::Vec2<T> resultPoint;
        double resultValue;
        Type type;
        std::vector<palka::Vec2<T>> target_plot;

    public:
        Plot<T> plot_perpendicular;

        GraphicsResult() = default;

        GraphicsResult(Type t, std::vector<palka::Vec2<T>> Union, const palka::Vec2<T>& resultPoint, T resultValue,
                       std::vector<palka::Vec2<T>> target_plot, Plot<T> plot_perpendicular) : Union(std::move(Union)),
                                                                                              resultPoint(resultPoint),
                                                                                              resultValue(resultValue),
                                                                                              type(t), target_plot(target_plot),
                                                                                              plot_perpendicular(plot_perpendicular)
        {}

        std::pair<std::vector<T>, std::vector<T>> getVisualUnion()
        {
#ifdef TracyProfiler
            ZoneScopedS(6);
#endif
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

        std::pair<std::vector<T>, std::vector<T>> getVisualTargetPlot()
        {
#ifdef TracyProfiler
            ZoneScopedS(6);
#endif
            std::vector<T> X;
            std::vector<T> Y;
            for(auto vec: target_plot)
            {
                X.emplace_back(vec.x);
                Y.emplace_back(vec.y);
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

        std::string serialize()
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            nlohmann::json json;
            for(auto& p: Union)
            {
                nlohmann::json vec;
                vec["x"] = p.x;
                vec["y"] = p.y;
                json["Points"].emplace_back(vec);
            }
            json["ResPoint"]["x"] = resultPoint.x;
            json["ResPoint"]["y"] = resultPoint.y;
            json["ResValue"] = resultValue;
            return json.dump(4);
        }

        void deserialize(const std::string& string)
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
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

        template<class T>
        auto solver(std::vector<std::vector<T>> matrix)
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            for(int col = 0; col < matrix.size() - 1; ++col)
            {
                T a = matrix[col][col];
                if(abs(a) != 1)
                {
                    for(T& j: matrix[col])
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
                std::vector<std::vector<T>> res;
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
                if(abs(a) != 1)
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
    class GraphicMet3D : public Solver<T, GraphicsResult<T>>
    {
        Plot3D<T> target;
        std::vector<Magpie::Plot3D<T>> Plots;
    public:
    protected:
        std::string serialize() override
        {
            return std::string();
        }

        void deserialize(const std::string& string) override
        {

        }

    public:
        GraphicMet3D() = default;

        void init(const MatrixStorage<T>& data, int var_count, int limitations_count)
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            Plots.clear();
            target = Magpie::Plot3D<T>(data.get(0, 0), data.get(1, 0), data.get(2, 0), 0);
            for(int i = 1; i < limitations_count + 1; ++i)
            {
                auto a = data.get(0, i);
                auto b = data.get(1, i);
                auto c = data.get(2, i);
                auto d = data.get(4, i);
                auto s = (int) data.get(3, i);
                Plots.emplace_back(Magpie::Plot3D<T>(a, b, c, d, (Sign) s));
            }
            Plots.emplace_back(Magpie::Plot3D<T>(1, 0, 0, 0, (Sign) Sign::GREATEROREQUAL));
            Plots.emplace_back(Magpie::Plot3D<T>(0, 1, 0, 0, (Sign) Sign::GREATEROREQUAL));
            Plots.emplace_back(Magpie::Plot3D<T>(0, 0, 1, 0, (Sign) Sign::GREATEROREQUAL));
        }

        std::vector<palka::Vec3<T>> points;

        auto findSolution()
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            T min = std::numeric_limits<T>::max();
            palka::Vec3<T> resVec;
            for(auto& vec: points_faces) //find min point and value
            {
                auto p = palka::Vec3<T>::convertFrom(vec.second);
                if(auto val = target.a * p.x + target.b * p.y + p.z * target.c;val < min)
                {
                    min = val;
                    resVec = p;
                }
            }
            return std::make_pair(resVec, min);
        }

        GraphicsResult<T> solve() override
        {

            std::vector<Ray<T>> rays;
            for(int i = 0; i < Plots.size() - 1; ++i) //find rays
            {
                for(int j = i + 1; j < Plots.size(); ++j)
                {
                    auto& v = Plots[i];
                    auto& v2 = Plots[j];
                    if(v != v2)
                    {
                        palka::Vec3<T> t1;
                        palka::Vec3<T> t2;
                        if(v.isect_plane_plane_to_normal_ray(v2, t1, t2))
                        {
                            rays.emplace_back(t1, t2);
                        }
                    }
                }
            }


            for(int i = 0; i < rays.size() - 1; ++i) //find points of ray intersection (unique)
            {
                for(int j = i + 1; j < rays.size(); ++j)
                {
                    auto& v = rays[i];
                    auto& v2 = rays[j];
                    if(v != v2)
                    {
                        if(auto res = v.intersects(v2); res != std::nullopt)
                        {
                            auto res_val = res.value();
                            if(std::ranges::find(points, res) == points.end())
                            {
                                for(auto& l: Plots)
                                {
                                    if(l.on_line(res_val))
                                    {
                                        points.emplace_back(res_val);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            auto find_delte = [](std::vector<palka::Vec3<T>>& points2)
            {
                T maxDist = std::numeric_limits<T>::min();
                int pi1 = -1;
                int pi2 = -1;
                for(int i = 0; i < points2.size(); ++i)
                {
                    for(int j = i + 1; j < points2.size(); ++j)
                    {
                        auto& p1 = points2[i];
                        auto& p2 = points2[j];
                        if(auto d = glm::distance(p1, p2); d > maxDist)
                        {
                            maxDist = d;
                            pi1 = i;
                            pi2 = j;
                        }
                    }
                }
                return std::vector<palka::Vec3<T>>{points2[pi1], points2[pi2]};
            };

            std::vector<palka::Vec3<T>> filtered_p;

            for(int i = 0; i < points.size() - 1; ++i) //delete duplicates
            {
                auto& v = points[i];
                for(auto& l: rays)
                {
                    if(l.containsPoint(v))
                    {
                        std::vector<palka::Vec3<T>> points2;
                        points2.emplace_back(v);
                        for(int j = 0; j < points.size(); ++j)
                        {
                            if(i == j)
                                continue;
                            auto& v2 = points[j];
                            if(l.containsPoint(v2))
                                points2.emplace_back(v2);
                        }
                        if(points2.size() >= 2)
                        {
                            auto vec = find_delte(points2);
                            std::move(vec.begin(), vec.end(), std::back_inserter(filtered_p));
                        }
                    }
                }
            }

            auto end = filtered_p.end();

            for(auto it = filtered_p.begin(); it != end; ++it)
                end = std::remove_if(it + 1, end, [&](const auto& elem) //the default comparison operator is not suitable
                {
                    return PlotCompare(elem.x, (*it).x) && PlotCompare(elem.y, (*it).y) && PlotCompare(elem.z, (*it).z);
                });

            filtered_p.erase(end, filtered_p.end());

            filtered_p = deleteBadPoints(filtered_p);

            std::vector<points_store < 3>>
            pointstriangles;
            std::vector<points_store < 4>>
            pointquads;
            for(size_t i = 0; i < filtered_p.size() - 1; ++i)
            {
                auto& p = filtered_p[i];
                for(auto& pl: Plots)
                {
                    if(pl.on_line(p))
                    {
                        std::vector<palka::Vec3<T>> pointsInSurf;
                        pointsInSurf.emplace_back(p);
                        for(size_t j = 0; j < filtered_p.size(); ++j)
                        {
                            auto& p2 = filtered_p[j];
                            if(i == j)
                                continue;
                            if(pl.on_line(p2))
                                pointsInSurf.emplace_back(p2);
                        }
                        if(pointsInSurf.size() == 4) //quads
                        {
                            pointquads.emplace_back(points_store < 4 > {{pointsInSurf[0], pointsInSurf[1], pointsInSurf[2], pointsInSurf[3]}, pl.DirNormal()});
                        } else if(pointsInSurf.size() == 3) //triangles
                        {
                            pointstriangles.push_back(points_store < 3 > {{pointsInSurf[0], pointsInSurf[1], pointsInSurf[2]}, pl.DirNormal()});
                        } else
                        {
                            palka::Console::addLog("Algorithm did not process the faces", palka::Console::logType::info);
                        }
                    }
                }
            }

//            for(size_t i = 0; i < filtered_p.size() - 1; ++i)
//            {
//                auto& p = filtered_p[i];
//                bool find = false;
//                for(size_t j = i + 1; j < filtered_p.size(); ++j)
//                {
//                    auto& p2 = filtered_p[j];
//                    for(auto& r: rays)
//                    {
//                        if(r.containsPoint(p) && r.containsPoint(p2))
//                        {
//                            std::swap(filtered_p[i + 1], filtered_p[j]);
//                            find = true;
//                            break;
//                        }
//                    }
//                    if(find)
//                        break;
//                }
//            }

            remove(pointquads);
            int face = 0;
            for(auto& q: pointquads)
            {
                auto qpoints = q.getPoints();
                points_faces.emplace(face, qpoints[0].convert());
                points_faces.emplace(face, qpoints[1].convert());
                points_faces.emplace(face, qpoints[2].convert());
                points_faces.emplace(face, qpoints[1].convert());
                points_faces.emplace(face, qpoints[2].convert());
                points_faces.emplace(face, qpoints[3].convert());
                normals.emplace_back(q.normal.convert());
//                pointstriangles.emplace_back(triangel<3>{{qpoints[0], qpoints[1], qpoints[2]}});
//                pointstriangles.emplace_back(triangel<3>{{qpoints[1], qpoints[2], qpoints[3]}});
                face++;
            }
            remove(pointstriangles);
            points.clear();
            for(auto& t: pointstriangles)
            {
                for(auto p: t.getPoints())
                {
                    //points.emplace_back(p);
                    points_faces.emplace(face, p.convert());
                }
                normals.emplace_back(t.normal.convert());
                face++;
            }

            auto vec = findSolution();
            return GraphicsResult<T>();
        }

        std::vector<palka::Vec3f> normals;
        std::multimap<int, palka::Vec3f> points_faces;

        template<int size>
        struct points_store
        {
            std::array<palka::Vec3<T>, size> points;
            palka::Vec3<T> normal;

            bool operator==(const points_store& ot) const
            {
                std::set<int> equals;
                for(auto& p: points)
                {
                    for(size_t i = 0; i < ot.points.size(); ++i)
                    {
                        auto& p2 = ot.points[i];
                        if(PlotCompare(p.x, p2.x) && PlotCompare(p.y, p2.y) && PlotCompare(p.z, p2.z))
                        {
                            if(!equals.emplace(i).second)
                            {
                                if(!equals.emplace(i).second)
                                    return false;
                            }
                        }
                    }
                }
                return equals.size() == size;
            }

            auto getPoints()
            {
                return points;
            }
        };

        std::vector<palka::Vec3<T>> deleteBadPoints(const std::vector<palka::Vec3<T>>& vec)
        {
            std::vector<palka::Vec3<T>> out;
            for(auto& p: vec)
            {
                bool all = true;
                for(auto& l: Plots)
                {
                    if(!l.containsPoint(p))
                    {
                        all = false;
                        break;
                    }
                }
                if(all)
                    out.emplace_back(p);
            }
            return out;
        }

        template<int t>
        void remove(std::vector<points_store<t>>& vec)
        {
            auto end = vec.end();
            for(auto it = vec.begin(); it != end; ++it)
                end = std::remove(it + 1, end, *it);
            vec.erase(end, vec.end());
        }
    };

    template<class T>
    class GraphicMet2D : public Solver<T, GraphicsResult<T>>
    {
    private:
        template<class>
        friend
        class GraphPrivateWrapper;

        Magpie::Plot<T> F;
        std::vector<Magpie::Plot<T>> Plots;
        GraphicsResult<T> result;
        std::vector<palka::Vec2<T>> Union;

        std::string serialize() override
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            nlohmann::json json;
            json["Func"]["a"] = F.a;
            json["Func"]["b"] = F.b;
            json["Func"]["c"] = F.c;
            json["Func"]["sign"] = signToStr(F.sign);
            for(auto& limit: Plots)
            {
                nlohmann::json json_limit;
                json_limit["Limit"]["a"] = limit.a;
                json_limit["Limit"]["b"] = limit.b;
                json_limit["Limit"]["c"] = limit.c;
                json_limit["Limit"]["sign"] = signToStr(limit.sign);
                json["Limits"].emplace_back(json_limit);
            }
            json["Result"] = result.serialize();
            return json.dump(4);
        }

        void deserialize(const std::string& string) override
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
        }

        static bool compare(const std::vector<Magpie::Plot<T>>& plots, const Magpie::Plot<T>& plot)
        {
            for(const auto& v: plots)
            {
                if(v == plot)
                    return true;
            }
            return false;
        }

        //in theory faster than sort
        static void sort2(std::vector<std::pair<palka::Vec2<T>, std::vector<Magpie::Plot<T>>>>& test)
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            std::set<size_t> hole_ids;
            for(size_t i = 0; i < test.size() - 1; ++i)
            {
                auto& p = test[i];
                bool find = false;
                for(size_t j = i + 1; j < test.size(); ++j)
                {
                    auto& p2 = test[j];
                    for(auto& plot: p.second)
                    {
                        if(plot.hasPoint(p2.first))
                        {
                            std::swap(test[i + 1], test[j]);
                            find = true;
                            break;
                        }
                    }
                    if(find)
                        break;
                }
                if(!find)
                {
                    hole_ids.emplace(i);
                    for(size_t j = i + 1; j < test.size(); ++j)
                    {
                        auto& p2 = test[j];
                        int hit = 0;
                        for(size_t s = 0; s < test.size(); ++s)
                        {
                            if(s == j) continue;
                            auto& p3 = test[s];
                            bool once = false;
                            for(auto& plot: p2.second)
                            {
                                if(plot.hasPoint(p3.first))
                                {
                                    hit++;
                                    break;
                                }
                            }
                        }
                        if(hit < 2)
                        {
                            hole_ids.emplace(i + 1);
                            std::swap(test[i + 1], p2);
                            break;
                        }
                    }
                }
                find = false;
            }

            if(hole_ids.size() == 2)
            {
                int index = std::max(*hole_ids.begin(), *std::next(hole_ids.begin(), 1));
                if(index > -1)
                    std::rotate(test.begin(),
                                test.end() - (test.size() - index),
                                test.end());
            }
        }

        static void sort(std::vector<palka::Vec2<T>>& points, const std::vector<Magpie::Plot<T>>& plots)
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            std::set<size_t> hole_ids;
            for(size_t i = 0; i < points.size() - 1; ++i)
            {
                auto& p = points[i];
                bool find = false;
                for(size_t j = i + 1; j < points.size(); ++j)
                {
                    auto& p2 = points[j];
                    for(auto& plot: plots)
                    {
                        if(plot.hasPoint(p) && plot.hasPoint(p2))
                        {
                            std::swap(points[i + 1], points[j]);
                            find = true;
                            break;
                        }
                    }
                    if(find)
                        break;
                }
                if(!find)
                {
                    hole_ids.emplace(i);
                    for(size_t j = i + 1; j < points.size(); ++j)
                    {
                        auto& p2 = points[j];
                        int hit = 0;
                        for(size_t s = 0; s < points.size(); ++s)
                        {
                            if(s == j) continue;
                            auto& p3 = points[s];
                            bool once = false;
                            for(auto& plot: plots)
                            {
                                if(plot.hasPoint(p3) && plot.hasPoint(p2))
                                {
                                    hit++;
                                    break;
                                }
                            }
                        }
                        if(hit < 2)
                        {
                            hole_ids.emplace(i + 1);
                            std::swap(points[i + 1], p2);
                            break;
                        }
                    }
                }
                find = false;
            }

            if(hole_ids.size() == 2)
            {
                int index = std::max(*hole_ids.begin(), *std::next(hole_ids.begin(), 1));
                if(index > -1)
                    std::rotate(points.begin(),
                                points.end() - (points.size() - index),
                                points.end());
            }
        }

    public:

        GraphicMet2D() = default;

        GraphicMet2D(const MatrixStorage<T>& data, int var_count, int limitations_count)
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
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
#ifdef TracyProfiler
            ZoneScoped;
#endif
            if(var_count > 2)
            {
                TestGetBazis test;
                auto res = test.solver(data.getMatrix());
                MatrixStorage<T> data2(data);
                data2.setMatrix(res.second);
                Plots.clear();
                F = Magpie::Plot<T>(data2.get(0, 0), data2.get(1, 0), 0);
                for(int i = 1; i < limitations_count + 1; ++i)
                {
                    auto a = data2.get(0, i);
                    auto b = data2.get(1, i);
                    auto c = data2.get(3, i);
                    auto s = (int) data2.get(2, i);
                    Plots.emplace_back(Magpie::Plot<T>{a, b, c, (Sign) s});
                }
                Plots.emplace_back(Magpie::Plot<T>(1, 0, 0, Sign::GREATEROREQUAL));
                Plots.emplace_back(Magpie::Plot<T>(0, 1, 0, Sign::GREATEROREQUAL));
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
//            Plots.emplace_back(Magpie::Plot<T>{1, 0, 0, Sign::GREATEROREQUAL});
//            Plots.emplace_back(Magpie::Plot<T>{0, 1, 0, Sign::GREATEROREQUAL});
        }

        auto findSolution()
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            T min = std::numeric_limits<T>::max();
            palka::Vec2<T> resVec;
            for(auto& vec: Union) //find min point and value
            {
                if(auto val = F.a * vec.x + F.b * vec.y;val < min)
                {
                    min = val;
                    resVec = vec;
                }
            }
            return std::make_pair(resVec, min);
        }

        //find all the intersection points
        auto getIntersectionPoints(const std::vector<Magpie::Plot<T>>& plots)
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            std::vector<palka::Vec2<T>> points;
            for(int i = 0; i < plots.size() - 1; ++i)
            {
                for(int j = i + 1; j < plots.size(); ++j)
                {
                    auto& v = plots[i];
                    auto& v2 = plots[j];
                    if(v != v2)
                    {
                        auto in = v.intersection(v2);
                        if(in.x != std::numeric_limits<T>::min() && in.y != std::numeric_limits<T>::min())
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
#ifdef TracyProfiler
            ZoneScoped;
#endif
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
                        filteredLines.emplace(v);
                        break;
                    }
                    all = true;
                }
            };
            for(int i = 0; i < plots.size() - 1; ++i)
            {
                for(int j = i + 1; j < plots.size(); ++j)
                {
                    auto& v = plots[i];
                    auto& v2 = plots[j];
                    if(v != v2)
                    {
                        auto in = v.intersection(v2);
                        if(in.x != std::numeric_limits<T>::min() && in.y != std::numeric_limits<T>::min())
                        {
                            deleteCheck(v, in);
                            deleteCheck(v2, in);
                        }
                    }
                }
            }
            return filteredLines;
        }

        GraphicsResult<T> solve() override
        {
#ifdef TracyProfiler
            ZoneScoped;
#endif
            std::vector<palka::Vec2<T>> points = getIntersectionPoints(Plots);
            std::set<Plot<T>> filteredLines = deleteBadPlots(Plots, points);
            Plots.clear();

            std::move(filteredLines.begin(), filteredLines.end(), std::back_inserter(Plots));

            std::vector<std::pair<palka::Vec2<T>, std::vector<Magpie::Plot<T>>>> maps;
            std::vector<Magpie::Plot<T>> plots_storage;
            for(auto& p: points) //discard those that do not lie in the domain of each function
            {
                bool all = true;
                plots_storage.clear();

                for(auto& val: Plots)
                {
                    if(!val.containsPoint(p))
                    {
                        all = false;
                        break;
                    }
                    if(val.hasPoint(p))
                        plots_storage.emplace_back(val);
                }
                if(all)
                {
                    if(std::find_if(Union.begin(), Union.end(), [&](const palka::Vec2<T> r)
                    {
                        return PlotCompare(r.x, p.x) && PlotCompare(r.y, p.y);
                    }) == Union.end())
                    {
                        Union.emplace_back(p.x, p.y);
                        maps.emplace_back(p, plots_storage);
                    }
                }
            }

            // GraphicMet2D<T>::sort2(maps);

            GraphicMet2D<T>::sort(Union, Plots);

            struct holes_store
            {
                palka::Vec2<T> p;
                std::vector<Plot<T>> plots;
            };

            std::vector<holes_store> breakPoints;
            if(Union.size() == 2)
            {
                std::vector<Plot<T>> plots1;
                std::vector<Plot<T>> plots2;
                for(auto& l: Plots)
                {
                    auto lp1 = l.hasPoint(Union[0]);
                    auto lp2 = l.hasPoint(Union[1]);
                    if(lp1 && lp2) continue;
                    if(lp1)
                        plots1.emplace_back(l);
                    if(lp2)
                        plots2.emplace_back(l);
                }
                breakPoints.emplace_back(holes_store{Union[0], plots1});
                breakPoints.emplace_back(holes_store{Union[1], plots2});
            } else
            {

/////////////////////////////////////////////////////NEW ALGO !!!!!!!!!!//////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//                for(int i = 0; i < maps.size(); ++i)
//                {
//                    auto& p = maps[i];
//                    int j = (i + 1 < maps.size()) ? i + 1 : 0;
//                    auto p2 = maps[j];
//
//                    std::vector<Plot<T>> plots1;
//                    std::vector<Plot<T>> plots2;
//                    bool once = false;
//
//                    for(auto& v1: p2.second)
//                    {
//                        if(std::find_if(p.second.begin(), p.second.end(), [&](auto& val)
//                        { return val == v1; }) != p.second.end())
//                        {
//                            once = true;
//                            break;
//                        }
//                    }
//                    if(!once)
//                    {
//                        holes.emplace_back(holes_store{p.first, p.second});
//                        holes.emplace_back(holes_store{p2.first, p2.second});
//                    }
//                }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
                        breakPoints.emplace_back(holes_store{p1, plots1});
                        breakPoints.emplace_back(holes_store{p2, plots2});
                    }
                }
            }
            std::vector<palka::Vec2<T>> trg_plot = {{F.getValueAtX(-20), -20},
                                                    {F.getValueAtX(20),  20}};
            if(!breakPoints.empty() && breakPoints.size() <= 2)
            {
                std::vector<std::pair<Plot<T>, palka::Vec2<T>>> pos;
                for(auto& h: breakPoints)
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
                                break;
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
                            if(plot.a == 0) //upscale point in a line
                            {
                                if(p.x < h.p.x)
                                    pos.emplace_back(plot, palka::Vec2<T>{p.x - 10, plot.getValueAtY(p.x - 10)});
                                else
                                    pos.emplace_back(plot, palka::Vec2<T>{p.x + 10, plot.getValueAtY(p.x + 10)});

                            } else
                            {
                                if(p.y < h.p.y)
                                    pos.emplace_back(plot, palka::Vec2<T>{plot.getValueAtX(p.y - 10), p.y - 10});
                                else
                                    pos.emplace_back(plot, palka::Vec2<T>{plot.getValueAtX(p.y + 10), p.y + 10});

                            }
                            break;
                        }
                    }
                }

                if(pos[0].first.hasPoint(Union.front()))
                {
                    Union.insert(Union.begin(), pos[0].second);
                    Union.insert(Union.end(), pos[1].second);
                } else
                {
                    Union.insert(Union.begin(), pos[1].second);
                    Union.insert(Union.end(), pos[0].second);
                }

                //checking that the gradient of the function does not look towards the hole
                auto& pl1 = pos[0].first;
                auto& pl2 = pos[1].first;

                auto vec1 = pos[0].second - breakPoints[0].p;
                auto vec2 = pos[1].second - breakPoints[1].p;
                auto vec = palka::Vec2<T>{-F.a, -F.b};

                auto check1 = glm::dot(vec, vec1);
                auto check2 = glm::dot(vec, vec2);


                if(check1 > 0 || check2 > 0) //the solution goes on infinity
                {
                    return result = GraphicsResult<T>(GraphicsResult<T>::open, Union, palka::Vec2<T>(), 0, trg_plot, F);
                } else//there is 1 solution
                {
                    auto resValue = findSolution();
                    return result = GraphicsResult<T>(GraphicsResult<T>::open, Union, resValue.first, resValue.second, trg_plot, F);
                }
            }

            auto resValue = findSolution();

            return result = GraphicsResult<T>(GraphicsResult<T>::closed, Union, resValue.first, resValue.second, trg_plot, F);
            return result;
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

        auto call2(std::vector<std::pair<palka::Vec2<T>, std::vector<Magpie::Plot<T>>>> test)
        {
            Magpie::GraphicMet2D<T>::sort2(test);
            return test;
        }
    };

#endif
}

#endif //BILLET_SOLVER_H
