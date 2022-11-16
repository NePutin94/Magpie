//
// Created by NePutin on 9/28/2021.
//
#ifndef PALKA_BUFFEROBJECT_H
#define PALKA_BUFFEROBJECT_H

#include <cstddef>
#include <glm/ext/matrix_float4x4.hpp>
#include "glad/gl.h"
#include <vector>

namespace palka
{
    class BufferObject
    {
    public:
        enum BufferType
        {
            NONE = -1,
            VBO = 0,
            UBO,
            EAB
        };
    protected:
        enum _BufferRenderType
        {
            STATIC,
            DYNAMIC,
            STREAM
        };

        unsigned int bufferID;
        _BufferRenderType renderType;
        BufferType bufferType;
        bool isCreated;
        size_t data_size;

        constexpr GLenum BufferTypeToGl()
        {
            constexpr GLenum v[] = {GL_ARRAY_BUFFER, GL_UNIFORM_BUFFER, GL_ELEMENT_ARRAY_BUFFER};
            return v[bufferType];
        }

        constexpr GLenum BufferRenderTypeToGl()
        {
            constexpr GLenum v[] = {GL_STATIC_DRAW, GL_DYNAMIC_DRAW, GL_STREAM_DRAW};
            return v[renderType];
        }

    public:
        using RenderType = _BufferRenderType;

        BufferObject() : bufferID(0), isCreated(false), renderType(STATIC), bufferType(NONE), data_size(0)
        {}

        BufferObject(BufferType type) : bufferID(0), isCreated(false), renderType(STATIC), bufferType(type), data_size(0)
        {}

        ~BufferObject()
        {
            deleteBuffer();
        }

        BufferObject(const BufferObject& b) = delete;

        BufferObject& operator=(BufferObject&& other) noexcept
        {
            if(this == &other)
                return *this;
            this->bufferID = other.bufferID;
            this->data_size = other.data_size;
            this->bufferType = other.bufferType;
            this->renderType = other.renderType;
            this->isCreated = other.isCreated;
            other.isCreated = false;
            other.bufferID = 0;
            return *this;
        }

        BufferObject(BufferObject&& other) noexcept
        {
            *this = std::move(other);
        }

        unsigned int getBufferID() const
        {
            return bufferID;
        }

        void create(size_t size = 0);

        void bind();

        void unbind();

        void deleteBuffer();

        void setData(const void* data, size_t data_size, size_t leftOffset);

        void setData(std::vector<glm::mat4> data, size_t leftOffset);

        void setData(std::vector<glm::vec3> data, size_t leftOffset)
        {
            glBindBuffer(BufferTypeToGl(), bufferID);
            auto _data_size = data.size() * sizeof(float[3]);
            setData(&data[0].x, _data_size, leftOffset);
//            if (this->data_size > _data_size)
//            {
//                this->data_size = _data_size;
//                glBufferData(BufferTypeToGl(), this->data_size, NULL, BufferRenderTypeToGl());
//            }
//            glBufferSubData(BufferTypeToGl(), leftOffset, _data_size, &data[0]);
//            glBindBuffer(BufferTypeToGl(), 0);
        }
    };
}
#endif //PALKA_BUFFEROBJECT_H
