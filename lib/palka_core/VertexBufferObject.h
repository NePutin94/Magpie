//
// Created by NePutin on 9/28/2021.
//

#ifndef PALKA_VERTEXOBJECTBUFFER_H
#define PALKA_VERTEXOBJECTBUFFER_H

#include "BufferObject.h"
#include "VertexData.h"
#include <glad/gl.h>

namespace palka
{
    class VertexBufferObject : public BufferObject
    {
    public:
        VertexBufferObject()
        {
            bufferType = VBO;
        }

        VertexBufferObject(VertexBufferObject&&) = default;

        VertexBufferObject& operator=(VertexBufferObject&&) = default;

        VertexBufferObject(BufferType t)
        {
            bufferType = t;
        }

        void setData(unsigned int data_size, const void* start, unsigned int offset = 0);
    };
}

#endif //PALKA_VERTEXOBJECTBUFFER_H
