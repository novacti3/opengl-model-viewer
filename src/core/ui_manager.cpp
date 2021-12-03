#include "ui_manager.hpp"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <pfd/portable-file-dialogs.h>

#include "log.hpp"
#include "resource_manager.hpp"
#include "../misc/utils.hpp"

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
            std::vector<std::string> shaderPaths = ShowFileDialog("Select shader", {"All files", "*", "Shader files", "*.vs *.fs"}, true);
            if(!shaderPaths.empty())
            {
                static struct FilePathSplitter
                {
                    std::pair<std::string, std::string> operator()(const std::string &path)
                    {
                        std::vector<std::string> splitPath = SplitString(path, '/');
                        std::vector<std::string> fileNameWithExtension = SplitString(splitPath[splitPath.size() - 1], '.');
                        std::string fileName = fileNameWithExtension[0];
                        std::string extension = fileNameWithExtension[1];

                        return make_pair(fileName, extension);
                    }
                } ParseFileNameAndExtension;

                using ShaderSourcePair = std::pair<std::string, std::string>;
                std::vector<ShaderSourcePair> shaderSources;

                // NOTE: A debug warning of sorts that says that a shader couldn't be created
                // because only one type of source file was provided would be cool to have
                using FileNameAndExtensionPair = std::pair<std::string, std::string>;
                int i = 0;
                while(shaderPaths.size() > 0)
                {
                    std::string path = shaderPaths[i];
                    FileNameAndExtensionPair fileNameAndExtension = ParseFileNameAndExtension(path);

                    for (const std::string &secondPath: shaderPaths)
                    {
                        FileNameAndExtensionPair secondFileNameAndExtension = ParseFileNameAndExtension(secondPath);

                        if(secondPath.compare(path) == 0)
                            continue;
                        
                        if(fileNameAndExtension.first.compare(secondFileNameAndExtension.first) == 0)
                        {
                            if(fileNameAndExtension.second.compare(secondFileNameAndExtension.second) != 0)
                            {
                                ShaderSourcePair newShader;
                                if(fileNameAndExtension.second.compare("vs") == 0)
                                    newShader = make_pair(path, secondPath);
                                else if(fileNameAndExtension.second.compare("fs") == 0)
                                    newShader = make_pair(secondPath, path);

                                if(std::find(shaderSources.begin(), shaderSources.end(), newShader) == shaderSources.end())
                                {
                                    shaderSources.push_back(newShader);
                                    shaderPaths.erase(std::find(shaderPaths.begin(), shaderPaths.end(), newShader.first));
                                    shaderPaths.erase(std::find(shaderPaths.begin(), shaderPaths.end(), newShader.second));
                                    break;
                                }
                            }
                        }
                    }
                    i = i < shaderPaths.size() ? i++ : shaderPaths.size();
                }

                for(const ShaderSourcePair &shaderSource: shaderSources)
                {
                    ResourceManager::getInstance().LoadShaderFromFiles(shaderSource.first, shaderSource.second);
                }
            }
        }

        ImGui::Separator();

        ImGui::Text("Shader uniforms:");
        // Shader uniforms display and editing
        for(ShaderUniform* const uniform: Scene::getInstance().shader->getUniforms())
        {
            switch(uniform->getType())
            {
                case ShaderUniformType::INT:
                    DrawWidgetInt(uniform->getName().c_str(), (int*)uniform->value);
                break;

                case ShaderUniformType::UINT:
                    DrawWidgetUnsignedInt(uniform->getName().c_str(), (unsigned int*)uniform->value);
                break;

                case ShaderUniformType::FLOAT:
                    DrawWidgetFloat(uniform->getName().c_str(), (float*)uniform->value);
                break;

                case ShaderUniformType::BOOL:
                    DrawWidgetCheckbox(uniform->getName().c_str(), (bool*)uniform->value);
                break;


                case ShaderUniformType::TEX2D:
                break;


                case ShaderUniformType::VEC2:
                    DrawWidgetVec2(uniform->getName().c_str(), (float*)uniform->value);
                break;

                case ShaderUniformType::VEC3:
                    DrawWidgetVec3(uniform->getName().c_str(), (float*)uniform->value);
                break;

                case ShaderUniformType::VEC4:
                    // TODO: Some sort of differenciation between regular vec4 and color
                    DrawWidgetColor(uniform->getName().c_str(), (float*)uniform->value);
                break;


                case ShaderUniformType::MAT2:
                break;

                case ShaderUniformType::MAT3:
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
void UIManager::DrawWidgetInt(const char* const label, int* const value)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    ImGui::DragInt("", value, 1);
}
void UIManager::DrawWidgetUnsignedInt(const char* const label, unsigned int* const value)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    ImGui::DragInt("", (int*)value, 1, 0, UINT_MAX, "%i");
}
void UIManager::DrawWidgetFloat(const char* const label, float* const value)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    ImGui::DragFloat("", value, 0.5f);
}
void UIManager::DrawWidgetCheckbox(const char* const label, bool* const value)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    ImGui::Checkbox("", value);
}

void UIManager::DrawWidgetVec2(const char* const label, float* const value)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    ImGui::DragFloat2("", value, 0.5f);
}
void UIManager::DrawWidgetVec3(const char* const label, float* const value)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    ImGui::DragFloat3("", value, 0.5f);
}
void UIManager::DrawWidgetVec4(const char* const label, float* const value)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    ImGui::DragFloat4("", value, 0.5f);
}
void UIManager::DrawWidgetColor(const char* const label, float* const value)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    ImGui::ColorEdit4("", value);
}
#pragma endregion