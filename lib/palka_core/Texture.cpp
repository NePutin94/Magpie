//
// Created by NePutin on 6/23/2021.
//

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include "ConsoleLog.h"

void palka::Texture::LoadFromFile(std::string_view path)
{
    assert(!valid);
    file_path = path;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int x, y, n;
    unsigned char* data = stbi_load(path.data(), &x, &y, &n, 0);
    if(data == NULL)
        Console::fmt_log("Unable to open file {}", Console::error, path);
    size.x = x;
    size.y = y;
    GLenum format;
    if(n == 1)
        format = GL_RED;
    else if(n == 3)
        format = GL_RGB;
    else if(n == 4)
        format = GL_RGBA;

    glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, data);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    valid = true;
}

palka::Texture::Texture(std::string_view path) : file_path(path)
{
    LoadFromFile(path);
}

#ifdef REFLECTION_CORE

#include <rttr/registration>

RTTR_REGISTRATION
{
    using namespace rttr;
    registration::class_<palka::Texture>("Texture")
            .constructor<>()
            .property_readonly("file path", &palka::Texture::getFilePath)
            .property_readonly("size", &palka::Texture::getSize);
}

#endif