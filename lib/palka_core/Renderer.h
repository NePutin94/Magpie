//
// Created by NePutin on 9/15/2021.
//

#ifndef PALKA_RENDERER_H
#define PALKA_RENDERER_H


#include "RenderContext.h"
#include "VertexData.h"
#include "Shader.h"
#include "Drawable.h"
#include "Viewport.h"
#include "VertexBuffer.h"
#include "TransformObject.h"
#include "Camera.h"
#include "VertexArrayObject.h"
#include "StaticMesh.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include "PlaneMesh.h"
#include "OrbitCamera.h"
#include "PolygonMesh.h"

namespace palka
{
    class Renderer
    {
    private:
        Viewport view;
        Vec2i size;
        OrbitCamera camera;
    public:
        Renderer() : view({0, 0, 0, 0}), camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0f, 0.0f), 15.0f, 3.0f, glm::pi<float>() * 0.5f, 0.0f)
        {}

        Renderer(Vec2i sz) : size(sz), view({0, 0, (float) sz.x, (float) sz.y}),
                             camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0f, 0.0f), 15.0f, 3.0f, glm::pi<float>() * 0.5f, 0.0f)
        {}

        Renderer(Renderer&& ot) : view({0, 0, 0, 0}),
                                  camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0f, 0.0f), 15.0f, 3.0f, glm::pi<float>() * 0.5f, 0.0f)
        {
            view = ot.view;
            size = ot.size;
            camera = ot.camera;
        }

        glm::mat4 getProjectionMatrix()
        {
            return glm::perspective(glm::radians(45.f), (float) size.x / (float) size.y, 0.1f, 500.0f);
        }

        auto getSize()
        {
            return size;
        }

        void setSize(Vec2i sz)
        {
            size = sz;
            view.setSize(Vec2f(size.x, size.y));
        }

        Viewport& getViewport()
        {
            return view;
        }

        OrbitCamera& getCamera()
        {
            return camera;
        }

        void setup()
        {
            glReset();
        }

        void glReset()
        {
            glMatrixMode(GL_PROJECTION);
            glBindTexture(GL_TEXTURE_2D, 0);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_BLEND);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }

        void clear(Color color = {0, 0, 0});

        void applyBlend(BlendMode mode)
        {
            glEnable(GL_BLEND);
            glBlendFuncSeparate(
                    BlendMode::enumToGlConstant(mode.colorSrcFactor), BlendMode::enumToGlConstant(mode.colorDstFactor),
                    BlendMode::enumToGlConstant(mode.alphaSrcFactor), BlendMode::enumToGlConstant(mode.alphaDstFactor));
            glBlendEquationSeparate(
                    BlendMode::enumToGlConstant(mode.colorEquation),
                    BlendMode::enumToGlConstant(mode.alphaEquation));
        }

        void applyView()
        {
            glViewport(0, 0, size.x, size.y);
            glMatrixMode(GL_PROJECTION);
            glLoadMatrixf(view.getView().getMatrix());
            glMatrixMode(GL_MODELVIEW);
        }

        class Line
        {
            glm::vec3 startPoint;
            glm::vec3 endPoint;
        public:
            palka::VertexArrayObject vao;
            palka::VertexBufferObject vbo;
            VertArray vertices;

            Line(glm::vec3 start, glm::vec3 end, Color color = Color(255, 255, 255)) : vertices(VertArray::Lines)
            {

                startPoint = start;
                endPoint = end;

                vertices.add({{start.x, start.y, start.z}, color, glm::vec2{99.f, 0.f}, glm::vec3{88.0, 0.0, 0.0}});
                vertices.add({{end.x, end.y, end.z}, color, glm::vec2{99.f, 0.f}, glm::vec3{0.0, 0.0, 88.0}});
            }

            void init()
            {
                vao.create(2);
                vbo.create(sizeof(Vertex) * 2);
                vbo.setData(sizeof(Vertex) * 2, &vertices[0].pos.x);
                vao.setPointers(vbo, sizeof(Vertex));
                vao.unbind();
            }
        };

        void drawLine(Line& lx, palka::RenderContext context)
        {
            auto& buffer = *context.getUBO();
            glm::mat4 projection = glm::mat4(1.0f);
            projection = getProjectionMatrix();
            auto _view = camera.getViewMatrix();

            auto& shader = *context.getShader();
            shader.bind();
            shader.setUniform("viewPos", camera.getEye());
            context();
            lx.vao.bind();

            buffer.setData(glm::value_ptr(projection), sizeof(float[16]), 0);
            buffer.setData(glm::value_ptr(_view), sizeof(float[16]), sizeof(float[16]));
            buffer.setData(glm::value_ptr(context.getTransform()), sizeof(float[16]), sizeof(float[16]) * 2);

            glDrawArrays(GL_LINES, static_cast<GLint>(0), lx.vao.getSize());
        }

        void draw(palka::PolygonMesh& m, palka::RenderContext context, Vec3f lightPos)
        {
            applyBlend(context.getBlend());
            auto& shader = *context.getShader();
            auto& buffer = *context.getUBO();
            glm::mat4 projection = glm::mat4(1.0f);
            projection = getProjectionMatrix();
            auto _view = camera.getViewMatrix();

            shader.bind();
            shader.setUniform("objectColor", Vec3f{0.2f, 0.1f, 0.9f});
            shader.setUniform("lightColor", Vec3f{1.f, 0.1f, 0.1f});
            shader.setUniform("lightPos", lightPos);
            shader.setUniform("viewPos", camera.getEye());
            context();

            buffer.setData(glm::value_ptr(projection), sizeof(float[16]), 0);
            buffer.setData(glm::value_ptr(_view), sizeof(float[16]), sizeof(float[16]));
            buffer.setData(glm::value_ptr(context.getTransform()), sizeof(float[16]), sizeof(float[16]) * 2);

            m.render();
        }

        void draw_t(palka::PolygonMesh& m, palka::RenderContext context, Vec3f lightPos, const Texture& t)
        {
            applyBlend(context.getBlend());
            auto& shader = *context.getShader();
            auto& buffer = *context.getUBO();
            glm::mat4 projection = glm::mat4(1.0f);
            projection = getProjectionMatrix();
            auto _view = camera.getViewMatrix();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, t.textureID);
            shader.bind();
            shader.setUniform("objectColor", Vec3f{0.2f, 0.1f, 0.9f});
            shader.setUniform("lightColor", Vec3f{1.f, 0.1f, 0.1f});
            shader.setUniform("lightPos", lightPos);
            shader.setUniform("viewPos", camera.getEye());
            context();

            buffer.setData(glm::value_ptr(projection), sizeof(float[16]), 0);
            buffer.setData(glm::value_ptr(_view), sizeof(float[16]), sizeof(float[16]));
            buffer.setData(glm::value_ptr(context.getTransform()), sizeof(float[16]), sizeof(float[16]) * 2);

            m.render();
        }

        void draw(palka::PlaneMesh& m, palka::RenderContext context, Vec3f lightPos)
        {
            applyBlend(context.getBlend());
            auto& shader = *context.getShader();
            auto& buffer = *context.getUBO();
            glm::mat4 projection = glm::mat4(1.0f);
            projection = getProjectionMatrix();
            auto _view = camera.getViewMatrix();

            shader.bind();
            shader.setUniform("objectColor", Vec3f{0.2f, 0.1f, 0.9f});
            shader.setUniform("lightColor", Vec3f{1.f, 0.1f, 0.1f});
            shader.setUniform("lightPos", lightPos);
            shader.setUniform("viewPos", camera.getEye());
            context();

            buffer.setData(glm::value_ptr(projection), sizeof(float[16]), 0);
            buffer.setData(glm::value_ptr(_view), sizeof(float[16]), sizeof(float[16]));
            buffer.setData(glm::value_ptr(context.getTransform()), sizeof(float[16]), sizeof(float[16]) * 2);

            m.render();
        }

        void draw(VertArray array, RenderContext context);

        void draw(StaticMesh& m, RenderContext context, Vec3f lightPos);

        void draw(Model& m, RenderContext context);

        //void draw(gltf_loader& m, RenderContext context, tinygltf::Model&, VertexArrayObject& vao);

        // void draw(assimp_loader& m, RenderContext context);

        void draw(VertexArrayObject& array, RenderContext context, Vec3f lightPos);

        void draw(unsigned int& vao, RenderContext context, int size);

        void draw(const Drawable& d)
        {
            d.draw(*this);
        }
    };
}

#endif //PALKA_RENDERER_H
