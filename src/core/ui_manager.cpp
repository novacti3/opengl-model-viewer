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

void UIManager::DrawUI(RendererSettings &rendererSettings, Shader* const shaderInUse)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();

    DrawRendererPropertiesWindow(rendererSettings);
    DrawShaderPropertiesWindow(shaderInUse);

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::DrawRendererPropertiesWindow(RendererSettings &rendererSettings)
{
    ImGui::Begin("Renderer properties");
    
    UIManager::DrawWidgetEnum("Render mode");
    UIManager::DrawWidgetColor("Background color", (float*)(&(rendererSettings.bgColor)));

    ImGui::End();
}
void UIManager::DrawShaderPropertiesWindow(Shader* const shader)
{
    ImGui::Begin("Shader properties");
    for(std::shared_ptr<ShaderUniform> uniform: shader->getUniforms())
    {
        switch(uniform.get()->getType())
        {
            case ShaderUniformType::VEC4:
                // TODO: Some sort of differenciation between regular vec4 and color
                UIManager::DrawWidgetColor(uniform.get()->getName().c_str(), (float*)uniform.get()->value);
            break;

            case ShaderUniformType::MAT4:
                // Draw a 4x4 InputFloat widget thing
            break;
        }
    }
    ImGui::End();
}

#pragma region Widgets
void UIManager::DrawWidgetEnum(const char* label, int* const items, const int &itemCount)
{
    ImGui::BeginGroup();
    
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label, ""); ImGui::SameLine();

    // TODO: Finish combo widget
    ImGui::Combo("", 0, items, itemCount);

    ImGui::EndGroup();
}

void UIManager::DrawWidgetVec3(const char* label, float* const value)
{
    ImGui::BeginGroup();
    
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label, ""); ImGui::SameLine();
    
    ImGui::Text("X", ""); ImGui::SameLine();
    ImGui::DragFloat("", value);

    ImGui::Text("Y", ""); ImGui::SameLine();
    ImGui::DragFloat("", value + sizeof(float));

    ImGui::Text("Z", ""); ImGui::SameLine();
    ImGui::DragFloat("", value + sizeof(float) * 2);

    ImGui::EndGroup();
}
void UIManager::DrawWidgetVec4(const char* label, float* const value)
{
    ImGui::BeginGroup();
    
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label, ""); ImGui::SameLine();
    
    ImGui::Text("X", ""); ImGui::SameLine();
    ImGui::DragFloat("", value);

    ImGui::Text("Y", ""); ImGui::SameLine();
    ImGui::DragFloat("", value + sizeof(float));

    ImGui::Text("Z", ""); ImGui::SameLine();
    ImGui::DragFloat("", value + sizeof(float) * 2);

    ImGui::Text("W", ""); ImGui::SameLine();
    ImGui::DragFloat("", value + sizeof(float) * 3);

    ImGui::EndGroup();
}

void UIManager::DrawWidgetColor(const char* label, float* const value)
{
    ImGui::BeginGroup();
    
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label, ""); ImGui::SameLine();
    ImGui::ColorEdit4("", value);

    ImGui::EndGroup();
}
#pragma endregion