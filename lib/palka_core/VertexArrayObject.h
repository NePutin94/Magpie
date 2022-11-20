//
// Created by NePutin on 9/28/2021.
//

#ifndef PALKA_VERTEXARRAYOBJECT_H
#define PALKA_VERTEXARRAYOBJECT_H

#include "VertexBufferObject.h"

namespace palka
{
    class VertexArrayObject
    {
    public:
        VertexArrayObject() = default;

        VertexArrayObject(VertexArrayObject&&) = default;

        VertexArrayObject& operator=(VertexArrayObject&&) = default;

        void create(int size)
        {
            this->size = size;
            glGenVertexArrays(1, &VAO);
            //glCreateVertexArrays(1, &VAO);
        }

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
            return size;
        }

        virtual void setPointers(VertexBufferObject& vbo, size_t type_size)
        {
            bind();
            vbo.bind();
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, type_size, 0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, type_size, (void*) (24));
            glEnableVertexAttribArray(1);

            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, type_size, (void*) (32));
            glEnableVertexAttribArray(2);

            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, type_size, (void*) (40));
            glEnableVertexAttribArray(3);

            vbo.unbind();
            unbind();
        }

        void setPointer_u(VertexBufferObject& vbo, int bind_point, int size_type, int type, size_t stride, size_t offset, bool normalized = false)
        {
            vbo.bind();
            glVertexAttribPointer(bind_point, size_type, type, normalized ? GL_TRUE : GL_FALSE, stride, (void*) offset);
            glEnableVertexAttribArray(bind_point);
            vbo.unbind();
        }

        void setPointer_u(VertexBufferObject& vbo, int bind_point, int size_type, int type, size_t stride, void* offset, bool normalized)
        {
            vbo.bind();
            glVertexAttribPointer(bind_point, size_type, type, normalized ? GL_TRUE : GL_FALSE, stride, offset);
            glEnableVertexAttribArray(bind_point);
            vbo.unbind();
        }

        void setPointer_dsa(VertexBufferObject& vbo, int bind_point, int size_type, int type, size_t offset, bool normalized)
        {
            glEnableVertexArrayAttrib(VAO, bind_point);
            glVertexArrayAttribFormat(VAO, bind_point, size_type, type, normalized ? GL_TRUE : GL_FALSE, offset);
            glVertexArrayAttribBinding(VAO, bind_point, 0);
        }

        void linkVBO(VertexBufferObject& vbo,size_t stride)
        {
            glVertexArrayVertexBuffer(VAO, 0, vbo.getBufferID(), 0, sizeof(stride));
        }

        void linkEBO(VertexBufferObject& ebo)
        {
            glVertexArrayElementBuffer(VAO, ebo.getBufferID());
        }
    private:
        unsigned int VAO;
        int size;
    };
}

#endif //PALKA_VERTEXARRAYOBJECT_H
