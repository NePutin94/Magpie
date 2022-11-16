//
// Created by NePutin on 9/28/2021.
//

#include "VertexBufferObject.h"

void palka::VertexBufferObject::setData(unsigned int data_size, const void* start, unsigned int offset)
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
