#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <string>
#include <iostream>

#include "core/log.hpp"
#include "core/resource_manager.hpp"
#include "core/ui_manager.hpp"
#include "rendering/renderer.hpp"
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

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str(), NULL, NULL);
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Log::LogFatal("glad failed to initialize. Exiting application...");
        return -1;
    }

    UIManager::getInstance().Init(window);
    Renderer::getInstance().Init();

    Shader *shader = ResourceManager::getInstance().CreateShaderFromFiles("../../../res/shaders/unlit-color.vs", "../../../res/shaders/unlit-color.fs");
    ResourceManager::getInstance().AddShader(shader, "unlit");
    auto resShader = ResourceManager::getInstance().GetShader("unlit");

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        Renderer::getInstance().DrawScene(resShader->get());
        UIManager::getInstance().DrawUI(Renderer::getInstance().settings, resShader->get());
        
        glfwSwapBuffers(window);
    }

    Renderer::getInstance().DeInit();
    UIManager::getInstance().DeInit();
    
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}