#include "ui_manager.hpp"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

static constexpr char* GLSL_VERSION = "#version 150"; 

void UIManager::Init(GLFWwindow* const window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);
}
void UIManager::DeInit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::DrawUI(Shader* const shaderInUse)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    DrawRendererPropertiesWindow();
    DrawShaderPropertiesWindow(shaderInUse);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::DrawRendererPropertiesWindow()
{

}
void UIManager::DrawShaderPropertiesWindow(Shader* const shader)
{
    ImGui::Begin("Shader properties");
    for(std::shared_ptr<ShaderUniform> uniform: shader->getUniforms())
    {
        switch(uniform.get()->getType())
        {
            case ShaderUniformType::VEC4:
                ImGui::ColorEdit4(uniform.get()->getName().c_str(), (float*)uniform.get()->value);
            break;

            case ShaderUniformType::MAT4:
                // Draw a 4x4 InputFloat widget thing
            break;
        }
    }
    ImGui::End();
}