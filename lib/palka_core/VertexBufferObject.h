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

        void setData(unsigned int data_size, const void* start, unsigned int offset = 0)
        {
//            if(this->data_size < data_size)
//            {
//                this->data_size = data_size;
//                glNamedBufferData(bufferID, this->data_size, start, BufferRenderTypeToGl());
//            }
//            glNamedBufferSubData(bufferID, offset, data_size, start);
            bind();
            if(this->data_size < data_size)
            {
                this->data_size = data_size;
                glBufferData(BufferTypeToGl(), this->data_size, start, BufferRenderTypeToGl());
            }
            glBufferSubData(BufferTypeToGl(), offset, data_size, start);
            // glBindBuffer(BufferTypeToGl(), 0);
        }
    };
}

#endif //PALKA_VERTEXOBJECTBUFFER_H
