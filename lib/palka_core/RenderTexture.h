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
        GLuint fbo = -1;
        GLuint rbo = -1;
        bool inited = false;
    public:

        RenderTexture() = default;

        RenderTexture(Vec2i size) : Renderer(size)
        {}

        RenderTexture(RenderTexture&& ot) : Renderer(std::move(ot))
        {
            if(this == &ot)
                return;
            tex = std::move(ot.tex);
            fbo = ot.fbo;
            rbo = ot.rbo;
            inited = ot.inited;

            ot.fbo = -1;
            ot.rbo = -1;
            ot.inited = false;
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
            inited = true;
        }

        Texture& getTexture()
        {
            return tex;
        }

        ~RenderTexture()
        {
            if(inited)
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
