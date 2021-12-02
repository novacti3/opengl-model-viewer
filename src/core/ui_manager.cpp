#include "ui_manager.hpp"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <pfd/portable-file-dialogs.h>

#include "resource_manager.hpp"

#include <utility>

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0]) 

static constexpr char* GLSL_VERSION = "#version 150"; 

void UIManager::Init(GLFWwindow* const window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    ImGui::StyleColorsDark();
    
    //_windowFlags |= ImGuiWindowFlags_NoCollapse;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);
}
void UIManager::DeInit()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::DrawUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    DrawMainMenuBar();
    if(_showRendererProperties)
        DrawRendererPropertiesWindow();
    if(_showShaderProperties)
        DrawShaderPropertiesWindow();
    
    #ifdef _DEBUG
    if(_showImGuiDemoWindow)
        ImGui::ShowDemoWindow();
    #endif

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::vector<std::string> UIManager::ShowFileDialog(const std::string &title, const std::vector<std::string> &filters, bool allowMultiSelect)
{
    return pfd::open_file(title, "", filters, allowMultiSelect).result();
}

#pragma region Menus
void UIManager::DrawMainMenuBar()
{
    ImGui::BeginMainMenuBar();

    if(ImGui::BeginMenu("File"))
    {
        if(ImGui::MenuItem("Open file...", "CTRL+O", false, false))
        {
            // Open a file dialogue to let the user load a mesh
            auto path = ShowFileDialog("Select mesh", {"All files", "*", "OBJ files", ".obj"});
            // TODO: Notify the ResourceManager to load the provided mesh
        }
        if(ImGui::MenuItem("Close file", "CTRL+N", false, false))
        {
            // Close the currently loaded mesh
            // TODO: Notify the ResourceManager to unload the currently used mesh
        }
        ImGui::Separator();
        if(ImGui::MenuItem("Exit", "ESC", false, false))
        {
            // Quit program
            // TODO: Notify the app to end
        }

        ImGui::EndMenu();
    }
    
    if(ImGui::BeginMenu("Windows"))
    {
        ImGui::MenuItem("Renderer properties", "", &_showRendererProperties, true);
        ImGui::MenuItem("Shader properties", "", &_showShaderProperties, true);
        #ifdef _DEBUG
        ImGui::Separator();
        ImGui::MenuItem("ImGui demo", "", &_showImGuiDemoWindow, true);
        #endif
     
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();
}

void UIManager::DrawRendererPropertiesWindow()
{
    if(ImGui::Begin("Renderer properties", &_showRendererProperties, _windowFlags))
    {
        static RendererSettings &rendererSettings = Renderer::getInstance().settings;

        UIManager::DrawWidgetColor("Background color", (float*)(&(rendererSettings.bgColor)));
        
        static bool renderWireframe = false;
        UIManager::DrawWidgetCheckbox("Draw wireframe", &renderWireframe);
        rendererSettings.renderMode = renderWireframe ? RenderMode::WIREFRAME : RenderMode::TRIANGLES;
    }
    ImGui::End();
}
void UIManager::DrawShaderPropertiesWindow()
{
    if(ImGui::Begin("Shader properties", &_showShaderProperties, _windowFlags))
    {
        static const LoadedShadersMap &loadedShaders = ResourceManager::getInstance().getLoadedShaders();

        static std::vector<std::string> loadedShaderNames;
        // Only update the vector if necesarry
        if(loadedShaderNames.size() != loadedShaders.size())
        {
            for (const auto &shader: loadedShaders)
            {
                // Only add the shader into the combo if it isn't present in the vector already
                auto it = std::find(loadedShaderNames.begin(), loadedShaderNames.end(), shader.first);
                if(it == loadedShaderNames.end())
                {
                    loadedShaderNames.push_back(shader.first);
                }
            }
        }
        static int currentShader = loadedShaderNames.size() - 1;
        
        ImGui::Text("Loaded shaders"); ImGui::SameLine();
        // NOTE: Maybe put this in a DrawWidgetCombo func of sorts
        if(ImGui::BeginCombo("", loadedShaderNames[currentShader].c_str()))
        {
            for(int i = 0; i < loadedShaders.size(); i++)
            {
                const std::string &shaderName = loadedShaderNames[i];
                const bool isSelected = (currentShader == i);
                if(ImGui::Selectable(shaderName.c_str(), isSelected))
                {
                    currentShader = i;
                    Scene::getInstance().shader = const_cast<Shader*>(ResourceManager::getInstance().GetShader(shaderName));
                }
            }
            ImGui::EndCombo(); 
        }
        ImGui::SameLine();
        if(ImGui::Button("..."))
        {
            std::vector<std::string> shaders = ShowFileDialog("Select shader", {"All files", "*", "Shader files", "*.vs *.fs"}, true);
            // TODO: Notify ResourceManager to create a shader with the provided files
        }

        ImGui::Separator();

        ImGui::Text("Shader uniforms:");
        // Shader uniforms display and editing
        for(std::shared_ptr<ShaderUniform> uniform: Scene::getInstance().shader->getUniforms())
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
    }
    ImGui::End();
}
#pragma endregion

#pragma region Widgets
void UIManager::DrawWidgetFloat(const char* const label, float* const value)
{
    // FIXME: The text label probably didn't work because BeginGroup
    // not only locks the X position of the elements but also because
    // it makes the elements share states like IsItemHovered
    // So yeah, delete the group stuff, maybe that'll fix things
    ImGui::BeginGroup();

    // ImGui::Text(label); ImGui::SameLine();
    ImGui::DragFloat(label, value, 0.5f);
    
    ImGui::EndGroup();
}
void UIManager::DrawWidgetCheckbox(const char* const label, bool* const value)
{
    ImGui::BeginGroup();
  
    // ImGui::Text(label); ImGui::SameLine();
    ImGui::Checkbox(label, value);
    
    ImGui::EndGroup();
}

void UIManager::DrawWidgetVec3(const char* const label, float* const value)
{
    ImGui::BeginGroup();
    
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    
    ImGui::Text("X"); ImGui::SameLine();
    ImGui::DragFloat("", value);

    ImGui::Text("Y"); ImGui::SameLine();
    ImGui::DragFloat("", value + sizeof(float));

    ImGui::Text("Z"); ImGui::SameLine();
    ImGui::DragFloat("", value + sizeof(float) * 2);

    ImGui::EndGroup();
}
void UIManager::DrawWidgetVec4(const char* const label, float* const value)
{
    ImGui::BeginGroup();
    
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    
    ImGui::Text("X"); ImGui::SameLine();
    ImGui::DragFloat("", value);

    ImGui::Text("Y"); ImGui::SameLine();
    ImGui::DragFloat("", value + sizeof(float));

    ImGui::Text("Z"); ImGui::SameLine();
    ImGui::DragFloat("", value + sizeof(float) * 2);

    ImGui::Text("W"); ImGui::SameLine();
    ImGui::DragFloat("", value + sizeof(float) * 3);

    ImGui::EndGroup();
}

void UIManager::DrawWidgetColor(const char* const label, float* const value)
{
    ImGui::BeginGroup();
    
    ImGui::AlignTextToFramePadding();
    //ImGui::Text(label); ImGui::SameLine();
    ImGui::ColorEdit4(label, value);

    ImGui::EndGroup();
}
#pragma endregion