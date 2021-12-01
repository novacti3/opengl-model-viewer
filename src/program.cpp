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


    glm::mat4 projMatrix = glm::perspective(45.0f, (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT, 0.1f, 100.0f);

    glm::mat4 viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, -3.0f));
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    glm::mat4 MVP = glm::mat4(1.0f);


    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        static float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        static float rotSpeed = 10.0f;
        static float rotation = sin(rotSpeed * deltaTime);
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        
        MVP = projMatrix * viewMatrix * modelMatrix;
        resShader->get()->SetUniform<glm::mat4>("u_MVP", &MVP);        
        Renderer::getInstance().DrawScene(resShader->get());
        // UIManager::getInstance().DrawUI(Renderer::getInstance().settings, resShader->get());

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(ImGui::Begin("Test"))
        {
            ImGui::DragFloat("Drag", &rotSpeed);
            ImGui::InputFloat("Input", &rotSpeed);
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    Renderer::getInstance().DeInit();
    UIManager::getInstance().DeInit();
    
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}