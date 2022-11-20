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
        GLuint rbo;
    public:
        RenderTexture(Vec2i size) : Renderer(size)
        {

        }

        void create()
        {
            auto size = getSize();
            tex.empty(size);
            setSize(size);
            glGenFramebuffers(1, &fbo);
            glBindFramebuffer(GL_FRAMEBUFFER, fbo);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex.textureID, 0);

            glGenRenderbuffers(1, &rbo);
            glBindRenderbuffer(GL_RENDERBUFFER, rbo);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, size.x,
                                  size.y); // use a single renderbuffer object for both a depth AND stencil buffer.
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); //

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
