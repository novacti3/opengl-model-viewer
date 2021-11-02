#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

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

    const char* glslVersion = "#version 150";
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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);
    bool showDemoWindow = true;

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

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (showDemoWindow)
            ImGui::ShowDemoWindow(&showDemoWindow);

        ImGui::Render();
        
        GL_CALL(glad_glClear(GL_COLOR_BUFFER_BIT));
        GL_CALL(glad_glClearColor(1.0f, 0.0f, 0.9f, 1.0f));

        GL_CALL(glad_glBindVertexArray(VAO));
        resShader->get()->Bind();
        // GL_CALL(glad_glDrawArrays(GL_TRIANGLES, 0, 3));
        GL_CALL(glad_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
        resShader->get()->Unbind();
        GL_CALL(glad_glBindVertexArray(0));
        

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }
    
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}