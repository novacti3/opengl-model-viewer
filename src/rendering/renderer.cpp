#include "renderer.hpp"

#include "../core/log.hpp"

void Renderer::Init()
{
    float vertices[] =
    {
        -0.5f, 0.5f, // top left
        -0.5f, -0.5f, // bot left
        0.5f, -0.5f, // bot right
        0.5f, 0.5f // top right
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };


    GL_CALL(glad_glGenVertexArrays(1, &VAO));
    GL_CALL(glad_glGenBuffers(1, &VBO));
    GL_CALL(glad_glGenBuffers(1, &EBO));

    GL_CALL(glad_glBindVertexArray(VAO));

    GL_CALL(glad_glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_CALL(glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

    GL_CALL(glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    GL_CALL(glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    GL_CALL(glad_glVertexAttribPointer(0, 2, GL_FLOAT, false, 2*sizeof(float), nullptr));
    GL_CALL(glad_glEnableVertexAttribArray(0));

    GL_CALL(glad_glBindVertexArray(0));
    GL_CALL(glad_glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void Renderer::DeInit()
{
    GL_CALL(glad_glDeleteBuffers(1, &VBO));
    GL_CALL(glad_glDeleteBuffers(1, &EBO));
    GL_CALL(glad_glDeleteVertexArrays(1, &VAO));
}

void Renderer::DrawScene(Shader* const shader)
{
    GL_CALL(glad_glClear(GL_COLOR_BUFFER_BIT));
    GL_CALL(glad_glClearColor(settings.bgColor.x, settings.bgColor.y, settings.bgColor.z, 1.0f));

    GL_CALL(glad_glBindVertexArray(VAO));
    shader->Bind();
    // GL_CALL(glad_glDrawArrays(GL_TRIANGLES, 0, 3));
    GL_CALL(glad_glDrawElements((GLenum)settings.renderMode, 6, GL_UNSIGNED_INT, 0));
    shader->Unbind();
    GL_CALL(glad_glBindVertexArray(0));
};