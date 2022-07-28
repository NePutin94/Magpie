//
// Created by NePutin on 9/25/2021.
//

#ifndef PALKA_VERTEXBUFFER_H
#define PALKA_VERTEXBUFFER_H

#include <glad/gl.h>
#include "VertexData.h"

namespace palka
{
    class VertexBuffer
    {
    public:
        void create(int size)
        {
            _size = size;
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * size, 0, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        void update(Vertex* v, int size, unsigned int shaderProgram);

        void bind()
        {
            glBindVertexArray(VAO);
        }

        void unbind()
        {
            glBindVertexArray(0);
        }

        int getSize()
        {
            return _size;
        }

    private:
        unsigned int VBO, VAO;
        int _size;
    };
}

#endif //PALKA_VERTEXBUFFER_H
