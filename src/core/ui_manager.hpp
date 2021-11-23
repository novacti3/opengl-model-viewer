#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../misc/singleton.hpp"
#include "../rendering/renderer.hpp"
#include "../rendering/shader.hpp"

class UIManager : public Singleton<UIManager>
{
    public:
    void Init(GLFWwindow* const window);
    void DeInit();
    
    void DrawUI(RendererSettings &rendererSettings, Shader* const shaderInUse);

    private:
    void DrawRendererPropertiesWindow(RendererSettings &rendererSettings);
    void DrawShaderPropertiesWindow(Shader* const shader);


    void DrawWidgetEnum(const char* label, int* const items, const int &itemCount);
    
    void DrawWidgetVec3(const char* label, float* const value);
    void DrawWidgetVec4(const char* label, float* const value);

    void DrawWidgetColor(const char* label, float* const value);
};