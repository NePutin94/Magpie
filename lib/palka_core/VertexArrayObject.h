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

        void create(int size);

        void bind();

        void unbind();

        int getSize()
        {
            return size;
        }

        virtual void setPointers(VertexBufferObject& vbo, size_t type_size);

        void setPointer_u(VertexBufferObject& vbo, int bind_point, int size_type, int type, size_t stride, size_t offset, bool normalized = false);

        void setPointer_u(VertexBufferObject& vbo, int bind_point, int size_type, int type, size_t stride, void* offset, bool normalized);

        void setPointer_dsa(VertexBufferObject& vbo, int bind_point, int size_type, int type, size_t offset, bool normalized);

        void linkVBO(VertexBufferObject& vbo,size_t stride);

        void linkEBO(VertexBufferObject& ebo);
    private:
        unsigned int VAO;
        int size;
    };
}

#endif //PALKA_VERTEXARRAYOBJECT_H
