#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#define LOG(x) std::cout << x << std::endl;

static constexpr unsigned int WINDOW_WIDTH = 1270; 
static constexpr unsigned int WINDOW_HEIGHT = 720;
static const std::string WINDOW_TITLE = "My OpenGL Program";

int main()
{
    if(!glfwInit())
    {
        LOG("GLFW failed to initialize...");
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE.c_str(), NULL, NULL);
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        LOG("glad failed to initialize...");
        return -1;
    }

    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    
    return 0;
}