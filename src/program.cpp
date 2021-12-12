#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <pfd/portable-file-dialogs.h>

#include <string>

#include "core/log.hpp"
#include "core/resource_manager.hpp"
#include "core/ui_manager.hpp"
#include "core/scene.hpp"
#include "rendering/renderer.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"

static constexpr unsigned int WINDOW_WIDTH = 1270; 
static constexpr unsigned int WINDOW_HEIGHT = 720;
static const std::string WINDOW_TITLE = "My OpenGL Program";

int main()
{
#ifdef _DEBUG
    Log::SetLogLevelFilter(LogLevel::Info);
#else
    Log::SetLogLevelFilter(LogLevel::Info);
#endif

    // GLFW init
    if(!glfwInit())
    {
        Log::LogFatal("GLFW failed to initialize. Exiting application...");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // TODO: Implement viewport scaling
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str(), NULL, NULL);
    glfwMakeContextCurrent(window);

    // glad init
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        Log::LogFatal("glad failed to initialize. Exiting application...");
        return -1;
    }

    // Check if the system has something to open file dialogs with
    if(!pfd::settings::available())
    {
        Log::LogFatal("No suitable file dialog backend available. Exiting application...");
        return -1;
    }

    // Resource loading
    Scene::getInstance().shader = ResourceManager::getInstance().LoadShaderFromFiles("../../../res/shaders/default.vs", "../../../res/shaders/default.fs");
    
    ResourceManager::getInstance().LoadTextureFromFile("../../../res/textures/ui_image_missing.jpg");
    ResourceManager::getInstance().LoadTextureFromFile("../../../res/textures/tex_missing.jpg");
    
    // Scene::getInstance().model = ResourceManager::getInstance().LoadModelFromOBJFile("../../../res/models/axe.obj");

    // Rendering init
    UIManager::getInstance().Init(window);
    Renderer::getInstance().Init();

    // MVP calculation
    // NOTE: The projection matrix should react to the changes in resolution
    // and change accordingly
    glm::mat4 projMatrix = glm::perspective(45.0f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.0f);

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, -1.25f, -5.0f));
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);

    glm::mat4 MVP = glm::mat4(1.0f);


    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    
    constexpr float rotSpeed = -1.0f;
    float rotation = 0.0f; 
    
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Delta time calculation
        static float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Spinning of cube
        rotation = sin(rotSpeed * currentTime);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        
        // Updating the MVP uniform of the currently used shader
        MVP = projMatrix * viewMatrix * modelMatrix;
        if(Scene::getInstance().shader != nullptr)
        {
            Scene::getInstance().shader->SetUniform("u_ModelMatrix", (void*)&modelMatrix);
            Scene::getInstance().shader->SetUniform("u_MVP", (void*)&MVP);  
        }
        
        // Render the scene and UI
        Renderer::getInstance().DrawScene();
        UIManager::getInstance().DrawUI();
        
        glfwSwapBuffers(window);
    }

    Renderer::getInstance().DeInit();
    UIManager::getInstance().DeInit();
    
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}