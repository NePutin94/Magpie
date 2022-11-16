//
// Created by NePutin on 9/28/2021.
//

#include "UniformBuffer.h"

void palka::UniformBuffer::bindToPoint(int p)
{
    bind();
    glBindBufferBase(GL_UNIFORM_BUFFER, p, bufferID);
    unbind();
}
