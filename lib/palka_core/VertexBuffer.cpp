//
// Created by NePutin on 9/25/2021.
//

#include "VertexBuffer.h"

void palka::VertexBuffer::update(palka::Vertex* v, int size, unsigned int shaderProgram)
{
    GLint vaCoords = glGetAttribLocation(shaderProgram, "aCoords");
    GLint vaColor = glGetAttribLocation(shaderProgram, "vertColor");
    GLint vaTexCoord = glGetAttribLocation(shaderProgram, "aTexCoord");
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    if(size > _size)
    {
        _size = size;
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * size, 0, GL_STATIC_DRAW);
    }
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * size, &v->pos.x);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*) (24));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (32));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
