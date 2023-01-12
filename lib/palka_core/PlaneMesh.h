#ifndef MAGPIE_PLANEMESH_H
#define MAGPIE_PLANEMESH_H

#include "VertexArrayObject.h"

namespace palka
{
    class PlaneMesh
    {
    private:
        std::array<glm::vec3, 6> plane;
        VertexArrayObject vao;
        VertexBufferObject vbo;
    public:
        static constexpr glm::vec3 normal{1.f, 0.f, 0.f};

        void init()//, Vec3f bottom_left,Vec3f bottom_right
        {
            // plane = {top_left, top_right, bottom_left};
            //,bottom_left, bottom_right,top_right
            plane = {glm::vec3(0, -0.5f, 0.5f), glm::vec3(0, 0.5f, 0.5f), glm::vec3(0, -0.5f, -0.5f),
                     glm::vec3(0, -0.5f, -0.5f), glm::vec3(0, 0.5f, -0.5f), glm::vec3(0, 0.5f, 0.5f)};

            palka::VertArray array;
            for(auto& e: plane)
                array.add(e);

            vao.create(array.getSize());
            vao.bind();

            vbo.create(sizeof(Vertex) * array.getSize());
            vbo.setData(sizeof(Vertex) * array.getSize(), &array[0].pos.x);

            vao.setPointers(vbo, sizeof(Vertex));
            vbo.unbind();
            vao.unbind();
        }

        void update(Vec3f top_left, Vec3f top_right, Vec3f bottom_left)
        {
            plane = {top_left, top_right, bottom_left};

            palka::VertArray array;
            for(auto& e: plane)
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

#endif //MAGPIE_PLANEMESH_H
