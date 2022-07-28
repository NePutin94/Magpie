//
// Created by NePutin on 9/7/2021.
//

#ifndef PALKA_RENDERCONTEXT_H
#define PALKA_RENDERCONTEXT_H

#include "BlendMode.h"
#include "Texture.h"
#include "Transform.h"
#include "ShaderProgram.h"

namespace palka
{
    class RenderContext
    {
    public:
        typedef void(* RendererCallback)(ShaderProgram&);

    private:
        ShaderProgram* shader;
        UniformBuffer* ubo;
        BlendMode blend;
        Mat4f model_mat;
        std::function<void(ShaderProgram&)> callback;
    public:

        RenderContext() : model_mat(Mat4f{1.f}), ubo(nullptr), shader(nullptr), callback(nullptr)
        {}

        explicit RenderContext(ShaderProgram* shader, UniformBuffer* buffer = nullptr, Mat4f model_mat = Mat4f{1.f},
                               std::function<void(ShaderProgram&)> callback = nullptr)
                : model_mat(model_mat), ubo(buffer), shader(shader), callback(callback)
        {}

        ShaderProgram* getShader()
        {
            return shader;
        }

        UniformBuffer* getUBO()
        {
            return ubo;
        }

        Mat4f getTransform() const
        {
            return model_mat;
        }

        BlendMode getBlend() const
        {
            return blend;
        }

        void operator()()
        {
            if(callback)
                callback(*shader);
        }

        friend class Window;
    };
}

#endif //PALKA_RENDERCONTEXT_H
