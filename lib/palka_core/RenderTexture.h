//
// Created by NePutin on 9/15/2021.
//

#ifndef PALKA_RENDERTEXTURE_H
#define PALKA_RENDERTEXTURE_H

#include "Renderer.h"

namespace palka
{
    class RenderTexture : public Renderer
    {
    private:
        Texture tex;
        GLuint fbo = 0;
    public:
        RenderTexture() : Renderer({0,0})
        {

        }

        void create(Vec2i size)
        {
            tex.empty(size);
            setSize(size);
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.textureID, 0);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        Texture& getTexture()
        {
            return tex;
        }
        ~RenderTexture()
        {
            glDeleteFramebuffers(1, &fbo);
        }

        void bind()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        }

        void unbind()
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    };
}

#endif //PALKA_RENDERTEXTURE_H
