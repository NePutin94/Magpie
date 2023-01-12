//
// Created by dimka on 1/12/2023.
//

#ifndef MAGPIE_GRAPHICSMETHOD3D_H
#define MAGPIE_GRAPHICSMETHOD3D_H

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
    class GraphicsResult3D
    {
    public:
        enum Result
        {
            none,
            hasSolution,
            hasNoSolution
        };
    private:
        std::vector<palka::Vec3f> normals;
        std::multimap<int, palka::Vec3f> points_faces;

    public:
        Plot<T> plot_perpendicular;

        GraphicsResult3D() = default;

    };

    template<class T>
    class GraphicMet3D : public Solver<T, GraphicsResult3D<T>>
    {

        std::vector<Magpie::Plot3D<T>> Plots;
    public:
        Plot3D<T> target;
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

        GraphicsResult3D<T> solve() override
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

            std::vector<points_store<3>> pointstriangles;
            std::vector<points_store<4>> pointquads;
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
                            pointquads.emplace_back(points_store<4>{{pointsInSurf[0], pointsInSurf[1], pointsInSurf[2], pointsInSurf[3]}, pl.DirNormal()});
                        } else if(pointsInSurf.size() == 3) //triangles
                        {
                            pointstriangles.push_back(points_store<3>{{pointsInSurf[0], pointsInSurf[1], pointsInSurf[2]}, pl.DirNormal()});
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
            return GraphicsResult3D<T>();
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
}
#endif //MAGPIE_GRAPHICSMETHOD3D_H
