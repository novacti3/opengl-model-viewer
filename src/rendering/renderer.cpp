#include "renderer.hpp"

#include <glm/vec2.hpp> 
#include <glm/vec3.hpp> 

#include "../core/log.hpp"


struct Vertex
{
    glm::vec3 position;
    glm::vec2 uv;

    Vertex(): position(glm::vec3(0.0f)), uv(glm::vec2(0.0f)){}
    Vertex(glm::vec3 position = glm::vec3(0.0f), glm::vec2 uv = glm::vec2(0.0f))
    {
        this->position = position;
        this->uv = uv;
    }

    Vertex(const Vertex& other)
    {
        this->position = other.position;
        this->uv = other.uv;
    }
    Vertex& operator=(const Vertex& other)
    {
        this->position = other.position;
        this->uv = other.uv;
        return *this;
    }

    Vertex(Vertex&& other)
    {
        this->position = std::move(other.position);
        this->uv = std::move(other.uv);
    }
    Vertex& operator=(Vertex&& other)
    {
        this->position = std::move(other.position);
        this->uv = std::move(other.uv);
        return *this;
    }

    ~Vertex()
    {
        position = glm::vec3(0.0f);
        uv = glm::vec2(0.0f);
    }
};

void Renderer::Init()
{
    // float vertices[] =
    // {
    //     -0.5f, 0.5f, // top left
    //     -0.5f, -0.5f, // bot left
    //     0.5f, -0.5f, // bot right
    //     0.5f, 0.5f // top right
    // };

    // unsigned int indices[] =
    // {
    //     0, 1, 2,
    //     2, 3, 0
    // };


    Vertex vertices[] = {
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f)),

        Vertex(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),

        Vertex(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),

        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),

        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),

        Vertex(glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0))
    };

    GL_CALL(glad_glGenVertexArrays(1, &VAO));
    GL_CALL(glad_glGenBuffers(1, &VBO));
    GL_CALL(glad_glGenBuffers(1, &EBO));

    GL_CALL(glad_glBindVertexArray(VAO));

    GL_CALL(glad_glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_CALL(glad_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), (void*)vertices, GL_STATIC_DRAW));

    // GL_CALL(glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    // GL_CALL(glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    GL_CALL(glad_glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3) + sizeof(glm::vec2), (void*)0));
    GL_CALL(glad_glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(glm::vec3) + sizeof(glm::vec2), (void*)(sizeof(glm::vec3))));
    GL_CALL(glad_glEnableVertexAttribArray(0));
    GL_CALL(glad_glEnableVertexAttribArray(1));

    GL_CALL(glad_glBindVertexArray(0));
    GL_CALL(glad_glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void Renderer::DeInit()
{
    GL_CALL(glad_glDeleteBuffers(1, &VBO));
    GL_CALL(glad_glDeleteBuffers(1, &EBO));
    GL_CALL(glad_glDeleteVertexArrays(1, &VAO));
}


void Renderer::DrawScene(Shader* const shader, Texture* const texture)
{
    GL_CALL(glad_glEnable(GL_DEPTH_TEST));
    
    GL_CALL(glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GL_CALL(glad_glClearColor(settings.bgColor.x, settings.bgColor.y, settings.bgColor.z, 1.0f));

    GL_CALL(glad_glPolygonMode(GL_FRONT_AND_BACK, (GLenum)settings.renderMode));

    if(texture)
        (*texture).Bind();
    
    GL_CALL(glad_glBindVertexArray(VAO));
    shader->Bind();
    GL_CALL(glad_glDrawArrays(GL_TRIANGLES, 0, 36));
    // GL_CALL(glad_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    shader->Unbind();
    GL_CALL(glad_glBindVertexArray(0));
    
    if(texture)
        (*texture).Unbind();
};