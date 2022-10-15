//
// Created by NePutin on 9/15/2021.
//

#include "Renderer.h"

void palka::Renderer::draw(palka::VertArray array, palka::RenderContext context)
{
    applyBlend(context.getBlend());
    Vertex* pointer = &array[0];
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2,
                    GL_FLOAT,
                    sizeof(Vertex),
                    &pointer->pos.x);

    glColorPointer(3,
                   GL_UNSIGNED_BYTE,
                   sizeof(Vertex),
                   &pointer->color.r);

    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &pointer->texCoord.x);

    glDrawArrays(VertArray::type_to_gl(array.getType()), static_cast<GLint>(0), array.getSize());
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//void palka::Renderer::draw(palka::assimp_loader& m, palka::RenderContext context)
//{
//    applyBlend(context.getBlend());
//    auto& shader = *context.getShader();
//    auto& buffer = *context.getUBO();
//
//    glm::mat4 projection = glm::mat4(1.0f);
//    projection = camera.getProjectionMatrix();
//    auto _view = camera.getViewMatrix();
//
//    shader.bind();
//    shader.setUniform("viewPos", camera.cameraPos);
//
//    buffer.setData(glm::value_ptr(projection), sizeof(float[16]), 0);
//    buffer.setData(glm::value_ptr(_view), sizeof(float[16]), sizeof(float[16]));
//    buffer.setData(glm::value_ptr(context.getTransform()), sizeof(float[16]), sizeof(float[16]) * 2);
//
//    m.render(*context.getShader());
//}

void palka::Renderer::draw(palka::VertexArrayObject& array, palka::RenderContext context, glm::vec3 lightPos)
{
    applyBlend(context.getBlend());
    auto& shader = *context.getShader();
    auto& buffer = *context.getUBO();

    array.bind();
    glm::mat4 projection = glm::mat4(1.0f);
    projection = camera.getProjectionMatrix();
    auto _view = camera.getViewMatrix();

    shader.bind();
    shader.setUniform("objectColor", Vec3f{0.9f, 0.1f, 0.20f});
    shader.setUniform("lightColor", Vec3f{0.9f, 0.9f, 1.0f});
    shader.setUniform("lightPos", lightPos);
    shader.setUniform("viewPos", camera.cameraPos);

    buffer.setData(glm::value_ptr(projection), sizeof(float[16]), 0);
    buffer.setData(glm::value_ptr(_view), sizeof(float[16]), sizeof(float[16]));
    buffer.setData(glm::value_ptr(context.getTransform()), sizeof(float[16]), sizeof(float[16]) * 2);

    glDrawArrays(GL_TRIANGLES, static_cast<GLint>(0), array.getSize());
}

void palka::Renderer::draw(palka::StaticMesh& m, palka::RenderContext context, Vec3f lightPos)
{
    applyBlend(context.getBlend());
    auto& shader = *context.getShader();
    auto& buffer = *context.getUBO();
    glm::mat4 projection = glm::mat4(1.0f);
    projection = camera.getProjectionMatrix();
    auto _view = camera.getViewMatrix();

    shader.bind();
    shader.setUniform("objectColor", Vec3f{0.2f, 0.1f, 0.9f});
    shader.setUniform("lightColor", Vec3f{1.f, 0.1f, 0.1f});
    shader.setUniform("lightPos", lightPos);
    shader.setUniform("viewPos", camera.cameraPos);
    context();

    buffer.setData(glm::value_ptr(projection), sizeof(float[16]), 0);
    buffer.setData(glm::value_ptr(_view), sizeof(float[16]), sizeof(float[16]));
    buffer.setData(glm::value_ptr(context.getTransform()), sizeof(float[16]), sizeof(float[16]) * 2);

    m.render();
}

//void palka::Renderer::draw(palka::gltf_loader& m, palka::RenderContext context, tinygltf::Model& mod, VertexArrayObject& vao)
//{
//    applyBlend(context.getBlend());
//    auto& shader = *context.getShader();
//    auto& buffer = *context.getUBO();
//    glm::mat4 projection = glm::mat4(1.0f);
//    projection = camera.getProjectionMatrix();
//    auto _view = camera.getViewMatrix();
//
//    shader.bind();
//    shader.setUniform("objectColor", Vec3f{0.2f, 0.1f, 0.9f});
//    shader.setUniform("lightColor", Vec3f{1.f, 0.1f, 0.1f});
//    shader.setUniform("viewPos", camera.cameraPos);
//    context();
//
//    buffer.setData(glm::value_ptr(projection), sizeof(float[16]), 0);
//    buffer.setData(glm::value_ptr(_view), sizeof(float[16]), sizeof(float[16]));
//    buffer.setData(glm::value_ptr(context.getTransform()), sizeof(float[16]), sizeof(float[16]) * 2);
//
//    m.drawModel(mod, vao);
//}

void palka::Renderer::draw(palka::Model& m, palka::RenderContext context)
{
    applyBlend(context.getBlend());
    auto& shader = *context.getShader();
    auto& buffer = *context.getUBO();
    glm::mat4 projection = glm::mat4(1.0f);
    projection = camera.getProjectionMatrix();
    auto _view = camera.getViewMatrix();

    shader.bind();
    shader.setUniform("objectColor", Vec3f{0.2f, 0.1f, 0.9f});
    shader.setUniform("lightColor", Vec3f{1.f, 0.1f, 0.1f});
    shader.setUniform("viewPos", camera.cameraPos);
    context();

    buffer.setData(glm::value_ptr(projection), sizeof(float[16]), 0);
    buffer.setData(glm::value_ptr(_view), sizeof(float[16]), sizeof(float[16]));
    buffer.setData(glm::value_ptr(context.getTransform()), sizeof(float[16]), sizeof(float[16]) * 2);

    m.render(shader);
}

void palka::Renderer::draw(unsigned int& vao, palka::RenderContext context,int size)
{
    applyBlend(context.getBlend());
    auto& shader = *context.getShader();
    auto& buffer = *context.getUBO();

    glBindVertexArray(vao);
    glm::mat4 projection = glm::mat4(1.0f);
    projection = camera.getProjectionMatrix();
    auto _view = camera.getViewMatrix();

    shader.bind();
    shader.setUniform("objectColor", Vec3f{0.9f, 0.1f, 0.20f});
    shader.setUniform("lightColor", Vec3f{0.9f, 0.9f, 1.0f});
  //  shader.setUniform("lightPos", lightPos);
    shader.setUniform("viewPos", camera.cameraPos);

    buffer.setData(glm::value_ptr(projection), sizeof(float[16]), 0);
    buffer.setData(glm::value_ptr(_view), sizeof(float[16]), sizeof(float[16]));
    buffer.setData(glm::value_ptr(context.getTransform()), sizeof(float[16]), sizeof(float[16]) * 2);

    glDrawArrays(GL_TRIANGLES, static_cast<GLint>(0), size);
}

void palka::Renderer::clear(palka::Color color)
{
    glClearColor(color.r, color.g, color.b, 255);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, size.x, size.y);
}


//
//void palka::Renderer::VAODraw2(palka::Mesh& m, palka::ShaderProgram& s, UniformBuffer& buff)
//{
//    RenderContext context;
//    applyBlend(context.blend);
//
//    glm::mat4 projection = glm::mat4(1.0f);
//    projection = camera.getProjectionMatrix();
//    auto _view = camera.getViewMatrix();
//
//    s.bind();
//    // glUseProgram(s.getId());
//
////    s.setValue("objectColor", {0.2f, 0.1f, 0.9f});
////    s.setValue("lightColor", {1.f, 0.1f, 0.1f});
////    s.setValue("lightPos", {-5.f, 0.f, -5.f});
////    s.setValue("viewPos", camera.cameraPos);
//
//    s.setUniform("objectColor", Vec3f{0.2f, 0.1f, 0.9f});
//    s.setUniform("lightColor", Vec3f{1.f, 0.1f, 0.1f});
//    s.setUniform("lightPos", Vec3f{-5.f, 0.f, -5.f});
//    s.setUniform("viewPos", camera.cameraPos);
//
//
//    buff.setData(glm::value_ptr(projection), 0, sizeof(float[16]));
//    buff.setData(glm::value_ptr(_view), sizeof(float[16]), sizeof(float[16]));
//    // s.updateUBO(glm::value_ptr(projection), 0, sizeof(float[16]));
//    //s.updateUBO(glm::value_ptr(_view), sizeof(float[16]), sizeof(float[16]));
//
//    glm::mat4 model = glm::mat4(1.0f);
//    model = glm::translate(model, {0, 0, 0});
//    // model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0 % 10, 0.5f));
//    //s.updateUBO(glm::value_ptr(model), sizeof(float[16]) * 2, sizeof(float[16]));
//    buff.setData(glm::value_ptr(model), sizeof(float[16]) * 2, sizeof(float[16]));
//    m.render(s);
//    glUseProgram(0);
//}
