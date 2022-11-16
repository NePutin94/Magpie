//
// Created by NePutin on 9/28/2021.
//

#ifndef PALKA_UNIFORMBUFFER_H
#define PALKA_UNIFORMBUFFER_H

#include <glad/gl.h>
#include "BufferObject.h"

namespace palka
{
    class UniformBuffer : public BufferObject
    {
    private:

    public:
        UniformBuffer()
        {
            renderType = STREAM;
            bufferType = UBO;
        }

        UniformBuffer(const UniformBuffer&) = delete;

        void bindToPoint(int p);

        /*void use()
        {
            glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
           // glBufferData(GL_UNIFORM_BUFFER, sizeof(float[16]) * 3, NULL, GL_STREAM_DRAW); //two float 4x4 matrices
            glBindBufferBase(GL_UNIFORM_BUFFER, 0, bufferID);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }

        void set(float* data, int L, int R)
        {
            glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
            glBufferSubData(GL_UNIFORM_BUFFER, L, R, data);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);

            glBindBuffer(GL_UNIFORM_BUFFER, bufferID);
            if (this->data_size < R)
            {
                this->data_size = R;
                glBufferData(GL_UNIFORM_BUFFER, this->data_size, NULL, GL_STREAM_DRAW);
            }
            glBufferSubData(GL_UNIFORM_BUFFER, L, R, data);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        }*/
    };
}

#endif //PALKA_UNIFORMBUFFER_H
