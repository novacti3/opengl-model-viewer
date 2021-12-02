#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <string>
#include <iostream>

#include "core/log.hpp"
#include "core/resource_manager.hpp"
#include "core/ui_manager.hpp"
#include "core/scene.hpp"
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

    Shader *shader = ResourceManager::getInstance().CreateShaderFromFiles("../../../res/shaders/default.vs", "../../../res/shaders/default.fs");
    ResourceManager::getInstance().AddShader(shader, "default");

    shader = ResourceManager::getInstance().CreateShaderFromFiles("../../../res/shaders/unlit-tex.vs", "../../../res/shaders/unlit-tex.fs");
    ResourceManager::getInstance().AddShader(shader, "unlit-tex");


    Texture *tex = ResourceManager::getInstance().CreateTextureFromFile("../../../res/textures/tile.jpg");
    ResourceManager::getInstance().AddTexture(tex, "tile");


    //auto resShader = ResourceManager::getInstance().GetShader("unlit-color");
    auto resShader = ResourceManager::getInstance().GetShader("unlit-tex");
    auto resTex = ResourceManager::getInstance().GetTexture("tile");

    Scene::getInstance().shader = ResourceManager::getInstance().GetShader("unlit-tex")->get();
    Scene::getInstance().textures.push_back(ResourceManager::getInstance().GetTexture("tile")->get());

    glm::mat4 projMatrix = glm::perspective(45.0f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.0f);

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    glm::mat4 MVP = glm::mat4(1.0f);


    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    
    constexpr float rotSpeed = -1.0f;
    float rotation = 0.0f; 
    
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        static float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        rotation = sin(rotSpeed * currentTime);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        
        MVP = projMatrix * viewMatrix * modelMatrix;
        Scene::getInstance().shader->SetUniform("u_MVP", (void*)&MVP);  

        Renderer::getInstance().DrawScene();
        UIManager::getInstance().DrawUI(Renderer::getInstance().settings, resShader->get());
        
        glfwSwapBuffers(window);
    }

    Renderer::getInstance().DeInit();
    UIManager::getInstance().DeInit();
    
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}