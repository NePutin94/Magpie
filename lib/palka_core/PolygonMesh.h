//
// Created by dimka on 12/2/2022.
//

#ifndef MAGPIE_POLYGONMESH_H
#define MAGPIE_POLYGONMESH_H

#include "VertexArrayObject.h"
#include <glm/gtx/vector_angle.hpp>
#include <random>

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

        void init(std::multimap<int, glm::vec3> points) //painted faces
        {
            inited = true;
            palka::VertArray array;
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> red(80, 255);
            std::uniform_int_distribution<int> green(80, 255);
            std::uniform_int_distribution<int> blue(80, 255);

            Color color(red(mt), green(mt), blue(mt));
            for(auto it = points.begin(), end = points.end(); it != end; it = points.upper_bound(it->first))
            {
                auto range = points.equal_range(it->first);
                for(auto f = range.first; f != range.second; ++f)
                    array.add(Vertex(f->second, color));

                color = Color(red(mt), green(mt), blue(mt));
            }
            vao.create(array.getSize());
            vao.bind();

            vbo.create(sizeof(Vertex) * array.getSize());
            vbo.setData(sizeof(Vertex) * array.getSize(), &array[0].pos.x);

            vao.setPointers(vbo, sizeof(Vertex));
            vbo.unbind();
            vao.unbind();
        }

        void init(std::vector<glm::vec3> points)
        {
            inited = true;
            palka::VertArray array;
            Color color(90, 50, 0);
            int triangle = 0;
            for(auto& e: points)
            {
                if(triangle > 2)
                {
                    if(color.r + 20 < 255)
                        color.r += 100;
                    else color.r = 20;
                    if(color.g + 20 < 255)
                        color.g += 50;
                    else color.g = 0;
                    if(color.b + 20 < 255)
                        color.b += 90;
                    else color.b = 100;
                    triangle = 0;
                }
                triangle++;
                array.add(Vertex(e, color));
            }

            vao.create(array.getSize());
            vao.bind();

            vbo.create(sizeof(Vertex) * array.getSize());
            vbo.setData(sizeof(Vertex) * array.getSize(), &array[0].pos.x);

            vao.setPointers(vbo, sizeof(Vertex));
            vbo.unbind();
            vao.unbind();
        }

        void update(std::vector<glm::vec3> points)
        {
            inited = true;
            palka::VertArray array;
            Color color(90, 150, 120);
            int triangle = 0;
            for(auto& e: points)
            {
                if(triangle > 2)
                {
                    if(color.r + 20 < 255)
                        color.r += 100;
                    else color.r = 20;
                    if(color.g + 20 < 255)
                        color.g += 50;
                    else color.g = 0;
                    if(color.b + 20 < 255)
                        color.b += 90;
                    else color.b = 100;
                    triangle = 0;
                }
                triangle++;
                array.add(Vertex(e, color));
            }

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
