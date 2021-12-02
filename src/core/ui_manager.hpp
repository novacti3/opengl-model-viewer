#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include "../misc/singleton.hpp"
#include "../rendering/renderer.hpp"
#include "../rendering/shader.hpp"

class UIManager : public Singleton<UIManager>
{
    private:
    ImGuiWindowFlags _windowFlags = 0;

    bool _showRendererProperties = false;
    bool _showShaderProperties = false;
    #ifdef _DEBUG
    bool _showImGuiDemoWindow = false;
    #endif

    public:
    void Init(GLFWwindow* const window);
    void DeInit();
    
    void DrawUI(RendererSettings &rendererSettings, Shader* const shaderInUse);


    private:
    std::vector<std::string> ShowFileDialog(const std::string &title, const std::vector<std::string> &filters = {"All files", "*"}, bool allowMultiSelect = false);

    void DrawMainMenuBar();
    void DrawRendererPropertiesWindow(RendererSettings &rendererSettings);
    void DrawShaderPropertiesWindow(Shader* const shader);

    void DrawWidgetFloat(const char* const label, float* const value);
    void DrawWidgetCheckbox(const char* const label, bool* const value);

    void DrawWidgetVec3(const char* const label, float* const value);
    void DrawWidgetVec4(const char* const label, float* const value);

    void DrawWidgetColor(const char* const label, float* const value);
};