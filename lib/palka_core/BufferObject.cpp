//
// Created by NePutin on 9/28/2021.
//

#include "BufferObject.h"

void palka::BufferObject::create(size_t size)
{
    //glCreateBuffers(1, &bufferID);
    glGenBuffers(1, &bufferID);
    bind();
    if(size)
    {
        //glNamedBufferData(bufferID, data_size, NULL, BufferRenderTypeToGl());
        data_size = size;
        glBufferData(BufferTypeToGl(), data_size, NULL, BufferRenderTypeToGl());
    }
    isCreated = true;
    unbind();
}

void palka::BufferObject::bind()
{
    glBindBuffer(BufferTypeToGl(), bufferID);
}

void palka::BufferObject::unbind()
{
    glBindBuffer(BufferTypeToGl(), 0);
}

void palka::BufferObject::setData(const void* data, size_t data_size, size_t leftOffset)
{
//            if(this->data_size < data_size)
//            {
//                this->data_size = data_size;
//                glNamedBufferData(bufferID, this->data_size, NULL, BufferRenderTypeToGl());
//            }
//            glNamedBufferSubData(bufferID, leftOffset, data_size, data);

    glBindBuffer(BufferTypeToGl(), bufferID);
    if(this->data_size < data_size)
    {
        this->data_size = data_size;
        glBufferData(BufferTypeToGl(), this->data_size, NULL, BufferRenderTypeToGl());
    }
    glBufferSubData(BufferTypeToGl(), leftOffset, data_size, data);
    glBindBuffer(BufferTypeToGl(), 0);
}

void palka::BufferObject::setData(std::vector<glm::mat4> data, size_t leftOffset)
{
    glBindBuffer(BufferTypeToGl(), bufferID);
    auto _data_size = data.size() * sizeof(float[16]);
    setData(&(data[0])[0][0], _data_size, leftOffset);
//            if (this->data_size > _data_size)
//            {
//                this->data_size = _data_size;
//                glBufferData(BufferTypeToGl(), this->data_size, NULL, BufferRenderTypeToGl());
//            }
//            glBufferSubData(BufferTypeToGl(), leftOffset, _data_size, &data[0]);
//            glBindBuffer(BufferTypeToGl(), 0);
}

void palka::BufferObject::deleteBuffer()
{
    if(!isCreated)
        return;
    glDeleteBuffers(1, &bufferID);
    isCreated = false;
}
