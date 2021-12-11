#pragma once

#include <glad/glad.h>
#include <glm/vec4.hpp>

#include "../misc/singleton.hpp"

#include "../core/scene.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "model.hpp"

enum class RenderMode
{
    TRIANGLES = GL_FILL,
    WIREFRAME = GL_LINE
};

struct RendererSettings
{
    RenderMode renderMode = RenderMode::TRIANGLES;
    glm::vec4 bgColor = glm::vec4(23.0f/255.0f, 22.0f/255.0f, 26.0f/255.0f, 1.0f);
};

class Renderer : public Singleton<Renderer>
{
    public:
    RendererSettings settings;

    private:
    Model *_cube;
    Model *_quad;

    public:
    void Init();
    void DeInit();
    void DrawScene();
};