//
// Created by NePutin on 6/23/2021.
//

#ifndef PALKA_TEXTURE_H
#define PALKA_TEXTURE_H

#include <string_view>
#include <string>
#include <cassert>
#include <glad/gl.h>

#include "Rect.h"

#ifdef REFLECTION_CORE

#include <rttr/type>
#include <rttr/registration_friend>

#endif

namespace palka
{
    class Texture
    {
#ifdef REFLECTION_CORE
        RTTR_ENABLE()
        RTTR_REGISTRATION_FRIEND
#endif
    private:
        std::string file_path;
        Vec2i size;
        bool flipped = false;
        bool valid = false;

    public:
        GLuint textureID = 0;

        Texture() = default;

        Texture& operator=(Texture&& other) noexcept
        {
            if(this == &other)
                return *this;
            textureID = other.textureID;
            valid = other.valid;
            size = other.size;
            file_path = std::move(other.file_path);
            other.valid = false;
            return *this;
        }

        Texture(Texture&& ot) noexcept
        {
            *this = std::move(ot);
        }

        Texture(const Texture&) = delete;

        Texture& operator=(const Texture&) = delete;

        explicit Texture(std::string_view path);

        void LoadFromFile(std::string_view path);

        void empty(Vec2i size)
        {
            this->size = size;
            valid = true;
            flipped = true;
            glGenTextures(1, &textureID);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        ~Texture()
        {
            if(valid)
                glDeleteTextures(1, &textureID);
        }

        void bind()
        {
            glBindTexture(GL_TEXTURE_2D, textureID);
            glMatrixMode(GL_TEXTURE);
            GLfloat matrix[16] = {1.f, 0.f, 0.f, 0.f,
                                  0.f, 1.f, 0.f, 0.f,
                                  0.f, 0.f, 1.f, 0.f,
                                  0.f, 0.f, 0.f, 1.f};
            if(flipped)
            {
                matrix[5] = -matrix[5];
                matrix[13] = 1;
            }
            glMatrixMode(GL_TEXTURE);
            glLoadMatrixf(matrix);
            glMatrixMode(GL_MODELVIEW);
        }

        Vec2i getSize() const
        {
            return size;
        }

        std::string_view getFilePath() const
        {
            return file_path;
        }

    };
}

#endif //PALKA_TEXTURE_H
