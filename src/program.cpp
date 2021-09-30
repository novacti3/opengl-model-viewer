#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#include "core/log.hpp"
#include "core/resource_manager.hpp"

#include "rendering/shader.hpp"

static constexpr unsigned int WINDOW_WIDTH = 1270; 
static constexpr unsigned int WINDOW_HEIGHT = 720;
static const std::string WINDOW_TITLE = "My OpenGL Program";

int main()
{
#ifdef _DEBUG
    Log::SetLogLevelFilter(LogLevel::Info);
#else
    Log::SetLogLevelFilter(LogLevel::Warning);
#endif

    if(!glfwInit())
    {
        Log::LogFatal("GLFW failed to initialize. Exiting application...");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str(), NULL, NULL);
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Log::LogFatal("glad failed to initialize. Exiting application...");
        return -1;
    }


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

    unsigned int VAO, VBO, EBO;

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


    Shader *shader = ResourceManager::getInstance().CreateShaderFromFiles("../../../res/shaders/unlit-color.vs", "../../../res/shaders/unlit-color.fs");
    ResourceManager::getInstance().AddShader(shader, "unlit");

    auto resShader = ResourceManager::getInstance().GetShader("unlit");

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glfwSwapBuffers(window);

        GL_CALL(glad_glClear(GL_COLOR_BUFFER_BIT));
        GL_CALL(glad_glClearColor(1.0f, 0.0f, 0.9f, 1.0f));

        GL_CALL(glad_glBindVertexArray(VAO));
        resShader->get()->Bind();
        // GL_CALL(glad_glDrawArrays(GL_TRIANGLES, 0, 3));
        GL_CALL(glad_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
        resShader->get()->Unbind();
        GL_CALL(glad_glBindVertexArray(0));
    }
    
    return 0;
}