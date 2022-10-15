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

        void empty(Vec2i size);

        ~Texture();

        void bind();

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
