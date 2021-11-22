#pragma once

#include <glad/glad.h>
#include <glm/vec3.hpp>

#include "../misc/singleton.hpp"

#include "shader.hpp"


enum class RenderMode
{
    TRIANGLES = GL_TRIANGLES,
    WIREFRAME = GL_LINES
};

struct RendererSettings
{
    RenderMode renderMode = RenderMode::TRIANGLES;
    glm::vec3 bgColor = glm::vec3(23.0f/255.0f, 22.0f/255.0f, 26.0f/255.0f);
};

class Renderer : public Singleton<Renderer>
{
    public:
    RendererSettings settings;

    private:
    unsigned int VAO, VBO, EBO;

    public:
    void Init();
    void DeInit();
    void DrawScene(Shader* const shader);
};