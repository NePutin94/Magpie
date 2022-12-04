//
// Created by dimka on 12/2/2022.
//

#ifndef MAGPIE_POLYGONMESH_H
#define MAGPIE_POLYGONMESH_H

#include "VertexArrayObject.h"
#include <glm/gtx/vector_angle.hpp>

namespace palka
{

    class PolygonMesh
    {
    private:
        std::vector<glm::vec3> polygon;
        VertexArrayObject vao;
        VertexBufferObject vbo;
        bool inited = false;
    public:
        bool isInit()
        {
            return inited;
        }

        void init(std::vector<glm::vec3> points)
        {
           // sortPoints(points);
            inited = true;
            palka::VertArray array;
            for(auto& e: points)
                array.add(e);

            vao.create(array.getSize());
            vao.bind();

            vbo.create(sizeof(Vertex) * array.getSize());
            vbo.setData(sizeof(Vertex) * array.getSize(), &array[0].pos.x);

            vao.setPointers(vbo, sizeof(Vertex));
            vbo.unbind();
            vao.unbind();
        }

        void sortPoints(std::vector<glm::vec3>& points)
        {
            palka::Vec3f center{0, 0, 0};
            for(auto& p: points)
            {
                center += p;
            }
            center /= palka::Vec3f{points.size(), points.size(), points.size()};
            for(auto& p: points)
            {
                p -= center;
            }
            std::ranges::sort(points, [&](palka::Vec3f p1, palka::Vec3f p2)
            {
                auto angle1 = glm::angle(center, p1);
                auto angle2 = glm::angle(center, p2);
                if(angle1 < angle2)
                    return true;
                auto d1 = glm::distance(center, p1);
                auto d2 = glm::distance(center, p2);
                if(angle1 == angle2 && d1 < d2)
                    return true;
                return false;
            });
            for(auto& p: points)
            {
                p += center;
            }
        }

        void update(std::vector<glm::vec3> points)
        {
           // sortPoints(points);
            inited = true;
            palka::VertArray array;
            for(auto& e: points)
                array.add(e);

            vao.bind();
            vbo.setData(sizeof(Vertex) * array.getSize(), &array[0].pos.x);
            vao.setPointers(vbo, sizeof(Vertex));
            vbo.unbind();
            vao.unbind();
        }

        void render()
        {
            vao.bind();
            glDrawArrays(GL_TRIANGLES, 0, vao.getSize());
            vao.unbind();
        }
    };
}

#endif //MAGPIE_POLYGONMESH_H
