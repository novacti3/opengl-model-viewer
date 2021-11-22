#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../misc/singleton.hpp"
#include "../rendering/shader.hpp"

class UIManager : public Singleton<UIManager>
{
    public:
    void Init(GLFWwindow* const window);
    void DeInit();
    
    void DrawUI(Shader* const shaderInUse);

    private:
    void DrawRendererPropertiesWindow();
    void DrawShaderPropertiesWindow(Shader* const shader);
};