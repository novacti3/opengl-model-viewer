#include "ui_manager.hpp"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <pfd/portable-file-dialogs.h>

#include "core/log.hpp"
#include "core/resource_manager.hpp"
#include "misc/utils.hpp"

#include <utility>

#define ARRAY_SIZE(x) sizeof(x)/sizeof(x[0]) 

static constexpr char* GLSL_VERSION = "#version 420"; 

void UIManager::Init(GLFWwindow* const window)
{
    // Init ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);
}
void UIManager::DeInit()
{
    // Deinit ImGui
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
    static ResourceManager &rm = ResourceManager::getInstance();
    static Scene &scene = Scene::getInstance();
    
    ImGui::BeginMainMenuBar();

    if(ImGui::BeginMenu("File"))
    {
        if(ImGui::MenuItem("Open file..."))
        {
            // Open a file dialogue to let the user load a mesh
            std::vector<std::string> paths = ShowFileDialog("Select mesh", {"All files", "*", "OBJ files", ".obj"});
            if(!paths.empty())
            {
                std::string path = paths[0];

                if(path.compare("") != 0)
                {
                    const auto &loadedModels = rm.getLoadedModels();
                    std::string currentModelName;
                    
                    for(const auto &model: loadedModels)
                    {
                        if(model.second == scene.model)
                        {
                            currentModelName = model.first;
                            break;
                        }
                    }

                    rm.UnloadModel(currentModelName);

                    scene.model = rm.LoadModelFromOBJFile(path);
                }
            }
        }
        if(ImGui::MenuItem("Close file"))
        {
            const auto &loadedModels = rm.getLoadedModels();
            std::string currentModelName;
            
            for(const auto &model: loadedModels)
            {
                if(model.second == scene.model)
                {
                    currentModelName = model.first;
                    break;
                }
            }

            if(currentModelName.compare("") != 0)
            {
                rm.UnloadModel(currentModelName);

                scene.model = nullptr;
            }
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
        
        static const LoadedShadersMap &loadedShaders = ResourceManager::getInstance().getLoadedShaders(); // Holds all of the actual Shader ptrs for later use
        static std::vector<std::string> loadedShaderNames; // Holds only the names of the shaders for use in the selection combo
        static int currentShader = 0; // The index of the current selected shader in the combo
        
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

                // Set the current shader index equal to this shader's place in the loaded names list if it's the currently used shader
                // Used to start the combo off on the right shader when first loading the window (eg. on "default" in most cases)
                if(shader.second == Scene::getInstance().shader)
                {
                    // currentShader = std::distance(loadedShaderNames.begin(), loadedShaderNames.end()) - 1;
                    currentShader = FindIndexOfElement<std::string>(loadedShaderNames, shader.first);
                }
            }
        }
        

        ImGui::Text("Loaded shaders"); ImGui::SameLine();
        ImGui::SetNextItemWidth(ImGui::GetWindowWidth()/2);
        // NOTE: Maybe put this in a DrawWidgetCombo func of sorts
        if(ImGui::BeginCombo("", loadedShaderNames[currentShader].c_str()))
        {
            for(int i = 0; i < loadedShaderNames.size(); i++)
            {
                const std::string &shaderName = loadedShaderNames[i];
                const bool isSelected = (currentShader == i);
                // If the shader is clicked, get the shader from the ResourceManager and have the scene use it to render stuff
                if(ImGui::Selectable(shaderName.c_str(), isSelected))
                {
                    currentShader = i;
                    Scene::getInstance().shader = const_cast<Shader*>(ResourceManager::getInstance().GetShader(shaderName));
                    
                    // Get rid of the list of textures used by the scene
                    // because the new shader may use a different number of them or none at all
                    auto &texturesInScene = Scene::getInstance().textures;
                    texturesInScene.clear();
                    
                    // Get the amount of shader uniforms of the TEX2D type and preallocate space in them in the Scene.textures vector with a blank Texture
                    // so that the values can get swapped out rather than pushed back to preserve texture binding target info (eg. bind tex to GL_TEXTURE2 etc.) 
                    // NOTE: a vector of pairs of ShaderUniformType and vector of ShaderUniform*
                    // might be a cool thing to implement into the shader so that there are is central
                    // list of each uniform type and their respective place in the uniforms list on shader create
                    // so that the entire list doesn't have to be looped over all the time
                    std::vector<ShaderUniform*> texUniforms = Scene::getInstance().shader->getUniformsOfType(ShaderUniformType::TEX2D);  
                    if(texturesInScene.size() != texUniforms.size())
                    {
                        for (int i = 0; i < texUniforms.size(); i++)
                        {
                            texturesInScene.push_back(new Texture());
                        }
                    }
                }
            }
            ImGui::EndCombo(); 
        }
        
        ImGui::SameLine();
        // Unload shader button
        if(ImGui::Button("X"))
        {
            const std::string &currentShaderName = loadedShaderNames[currentShader];
            // Unload the shader only if it's not the default shader because it doesn't make sense to delete a DEFAULT shader
            if(currentShaderName != "default")
            {
                Scene::getInstance().shader = nullptr;
                ResourceManager::getInstance().UnloadShader(currentShaderName);
                
                // Set the new current shader index equal to the default shader because it's always guaranteed to be present
                int defaultShaderIndex = FindIndexOfElement<std::string>(loadedShaderNames, "default");
                currentShader = defaultShaderIndex != -1 ? defaultShaderIndex : 0;

                // Erase the name of the now unloaded shader from the combo selection because it doesn't make sense
                // to be able to select something that doesn't exist anymore
                auto it = std::find(loadedShaderNames.begin(), loadedShaderNames.end(), currentShaderName);
                loadedShaderNames.erase(it);
            }
        }
        
        ImGui::SameLine();
        // Load new shader button
        if(ImGui::Button("..."))
        {
            std::vector<std::string> shaderPaths = ShowFileDialog("Select shader", {"All files", "*", "Shader files", "*.vs *.fs"}, true);
            // Only proceed with loading the shaders if the user selected any shader files to begin with
            if(!shaderPaths.empty())
            {
                using FileNameAndExtensionPair = std::pair<std::string, std::string>;
                using ShaderSourcePair = std::pair<std::string, std::string>;
                std::vector<ShaderSourcePair> shaderSources;

                // Loop through the provided shader paths until there aren't any left to create shaders from
                int i = 0;
                while(shaderPaths.size() > 0)
                {
                    std::string path = shaderPaths[i];
                    
                    // Only parse the file if there's a valid path leading to it
                    if(path.empty() || path.compare("") == 0)
                        continue;
                    
                    // Only continue with the parsing if a file with shader code was provided.
                    // It doesn't make sense to load shaders from images for example.
                    FileNameAndExtensionPair fileNameAndExtension = ResourceManager::ParseFileNameAndExtension(path);
                    if(fileNameAndExtension.second.compare("vs") != 0 && fileNameAndExtension.second.compare("fs") != 0)
                    {
                        Log::LogError("Shader loading failed, please provide a file of a shader file type (vs, fs)\n Provided file: " + path);
                        shaderPaths.erase(std::find(shaderPaths.begin(), shaderPaths.end(), path));
                        continue;
                    }

                    // Create a path to the second shader but emit the file extension from it.
                    // This is important in the next step because depending on what kind of shader the current path is, the opposite shader type needs to be found for it 
                    // (eg. if we have a path to a vertex shader right now, we need to look for the fragment shader)
                    std::string secondShaderPath = path;
                    secondShaderPath = path.substr(0, path.length() - 3);

                    ShaderSourcePair newShader;
                    if(fileNameAndExtension.second == "fs")
                    {
                        secondShaderPath += ".vs";
                        // Check if the path to the vertex shader is present. If not, replace it with an empty string
                        auto it = std::find(shaderPaths.begin(), shaderPaths.end(), secondShaderPath);

                        if(it != shaderPaths.end())
                        {
                            newShader = newShader = make_pair(secondShaderPath, path);
                        }
                        else
                            newShader = newShader = make_pair("", path);
                    }
                    else if(fileNameAndExtension.second == "vs")
                    {
                        // Check if the path to the fragment shader is present. If not, replace it with an empty string
                        secondShaderPath += ".fs";
                        auto it = std::find(shaderPaths.begin(), shaderPaths.end(), secondShaderPath);

                        if(it != shaderPaths.end())
                        {
                            newShader = newShader = make_pair(path, secondShaderPath);
                        }
                        else
                            newShader = newShader = make_pair(path, "");
                    }

                    // Only add the potential new shader to the list of shaders to-be-created if it's not already present
                    // and remove the paths to its shader files from the list of shader paths needed to be checked.
                    // Removing them allows the loop to finish execution and not end up infinite
                    if(std::find(shaderSources.begin(), shaderSources.end(), newShader) == shaderSources.end())
                    {
                        shaderSources.push_back(newShader);
                        
                        if(newShader.first != "")
                            shaderPaths.erase(std::find(shaderPaths.begin(), shaderPaths.end(), newShader.first));
                        
                        if(newShader.second != "")
                            shaderPaths.erase(std::find(shaderPaths.begin(), shaderPaths.end(), newShader.second));
                    }

                    i = i < shaderPaths.size() ? i++ : shaderPaths.size();
                }

                // Load all of the new shaders if they are complete (if they have paths to both a .vs and .fs file)
                for(const ShaderSourcePair &shaderSource: shaderSources)
                {
                    if(shaderSource.first.empty() || shaderSource.second.empty())
                    {
                        FileNameAndExtensionPair fileNameAndExtension = !shaderSource.first.empty() ? ResourceManager::ParseFileNameAndExtension(shaderSource.first) : ResourceManager::ParseFileNameAndExtension(shaderSource.second);
                        Log::LogError("Cannot load shader '" + fileNameAndExtension.first + "', only " + fileNameAndExtension.second + " shader provided");
                        continue;
                    }

                    ResourceManager::getInstance().LoadShaderFromFiles(shaderSource.first, shaderSource.second);
                }
            }
        }

        ImGui::Separator();

        ImGui::Text("Shader uniforms:");
        if(Scene::getInstance().shader != nullptr)
        {
            // Shader uniforms display and editing
            const std::vector<ShaderUniform*> &shaderUniforms = Scene::getInstance().shader->getUniforms();          
            // NOTE: a vector of pairs of ShaderUniformType and vector of ShaderUniform*
            // might be a cool thing to implement into the shader so that there are is central
            // list of each uniform type and their respective place in the uniforms list on shader create
            // so that the entire list doesn't have to be looped over all the time
            std::vector<ShaderUniform*> texUniforms = Scene::getInstance().shader->getUniformsOfType(ShaderUniformType::TEX2D);  

            // Draw the appropriate UI control widget for each uniform present in the shader given its type
            for(ShaderUniform* const uniform: shaderUniforms)
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


                    case ShaderUniformType::VEC2:
                        DrawWidgetVec2(uniform->getName().c_str(), (float*)uniform->value);
                    break;

                    case ShaderUniformType::VEC3:
                        DrawWidgetVec3(uniform->getName().c_str(), (float*)uniform->value);
                    break;

                    case ShaderUniformType::VEC4:
                        // NOTE: Some sort of differenciation between regular vec4 and color would be great
                        DrawWidgetColor(uniform->getName().c_str(), (float*)uniform->value);
                    break;


                    case ShaderUniformType::MAT2:
                    break;

                    case ShaderUniformType::MAT3:
                    break;

                    case ShaderUniformType::MAT4:
                        // Draw a 4x4 InputFloat widget thing
                    break;


                    case ShaderUniformType::TEX2D:
                        // unsigned int texBindTarget = std::distance(texUniforms.begin(), std::find(texUniforms.begin(), texUniforms.end(), uniform));

                        // Set the bind target of the current texture uniform to be its place in the list of uniforms of type TEX2D
                        // This means that textures will get the bind target by the way they are declared (eg. the first declared sampler2D uniform will be GL_TEXTURE0, the next one GL_TEXTURE1 and so on)
                        unsigned int texBindTarget = FindIndexOfElement<ShaderUniform*>(texUniforms, uniform);
                        Texture *newTex = DrawWidgetTex2D(uniform->getName().c_str(), (Texture*)uniform->value, texBindTarget);
                        // If a new texture was loaded using the Tex2D widget, delete the default empty texture and set the uniform's value to be the newly loaded texture
                        if(newTex != nullptr)
                        {
                            if(((Texture*)(uniform->value))->getID() == 0)
                            {
                                delete((Texture*)(uniform->value));
                            }
                            uniform->value = (void*)newTex;
                        }
                    break;
                }
            }
        }
    }
    ImGui::End();
}
#pragma endregion

#pragma region Widgets
#pragma region Base types
void UIManager::DrawWidgetInt(const char* const label, int* const value)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    std::string widgetID = "Int" + std::string(label);
    ImGui::PushID(widgetID.c_str());
    ImGui::DragInt("", value, 1);
    ImGui::PopID();
}
void UIManager::DrawWidgetUnsignedInt(const char* const label, unsigned int* const value)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    std::string widgetID = "UInt" + std::string(label);
    ImGui::PushID(widgetID.c_str());
    ImGui::DragInt("", (int*)value, 1, 0, UINT_MAX, "%i");
    ImGui::PopID();
}
void UIManager::DrawWidgetFloat(const char* const label, float* const value)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    std::string widgetID = "Float" + std::string(label);
    ImGui::PushID(widgetID.c_str());
    ImGui::DragFloat("", value, 0.5f);
    ImGui::PopID();
}
void UIManager::DrawWidgetCheckbox(const char* const label, bool* const value)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    std::string widgetID = "Checkbox" + std::string(label);
    ImGui::PushID(widgetID.c_str());
    ImGui::Checkbox("", value);
    ImGui::PopID();
}
#pragma endregion

#pragma region Vectors
void UIManager::DrawWidgetVec2(const char* const label, float* const value)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    std::string widgetID = "Vec2" + std::string(label);
    ImGui::PushID(widgetID.c_str());
    ImGui::DragFloat2("", value, 0.5f);
    ImGui::PopID();
}
void UIManager::DrawWidgetVec3(const char* const label, float* const value)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    std::string widgetID = "Vec3" + std::string(label);
    ImGui::PushID(widgetID.c_str());
    ImGui::DragFloat3("", value, 0.5f);
    ImGui::PopID();
}
void UIManager::DrawWidgetVec4(const char* const label, float* const value)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    std::string widgetID = "Vec4" + std::string(label);
    ImGui::PushID(widgetID.c_str());
    ImGui::DragFloat4("", value, 0.5f);
    ImGui::PopID();
}
void UIManager::DrawWidgetColor(const char* const label, float* const value)
{
    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    std::string widgetID = "Color" + std::string(label);
    ImGui::PushID(widgetID.c_str());
    ImGui::ColorEdit4("", value);
    ImGui::PopID();
}
#pragma endregion

Texture* UIManager::DrawWidgetTex2D(const char* const label, Texture* const value, unsigned int bindTarget)
{
    auto &texturesInScene = Scene::getInstance().textures; // List of the currently loaded textures in the scene
    auto &loadedTextures = ResourceManager::getInstance().getLoadedTextures(); // List of all the available textures loaded by ResourceManager
    
    static const Texture &missingImgTex = *(ResourceManager::getInstance().GetTexture("ui_image_missing"));
    static const Texture &missingTex = *(ResourceManager::getInstance().GetTexture("tex_missing"));
    static ImVec2 imgSize(128.0f, 128.0f); // Texture button/img preview size

    // The ptr to the returned tex. This can either be nullptr or, ptr to a newly loaded texture or the missing texture
    // depending on whether or not a texture was loaded using the appropriate load tex button
    Texture *returnedTex = nullptr;

    ImGui::AlignTextToFramePadding();
    ImGui::Text(label); ImGui::SameLine();
    
    // If the texture is not empty and has a value, use that as the image preview
    // otherwise use the appropriate "texture is missing" image
    void *img = nullptr; 
    if(value->getID() != 0 && value->getID() != missingTex.getID())
    {
        img = (void*)value->getID();
    }
    else
        img = (void*)missingImgTex.getID();
    
    std::string imgButtonID = "ImgButton" + std::string(label);
    ImGui::PushID(imgButtonID.c_str());
    // Load new tex button
    if(ImGui::ImageButton(img, imgSize))
    {
        auto pathsVector = ShowFileDialog("Select texture", {"All files", "*", "Image files", "*.jpg *.png"});
        // Only attempt to load a new texture if the user provided an image file to begin with
        if(!pathsVector.empty())
        {
            std::string path = pathsVector[0];

            Texture *newTex = ResourceManager::getInstance().LoadTextureFromFile(path);
            // If texture loading was successful, replace the texture at the specified bind index in the list of textures in the scene.
            // In case there isn't a texture at the bind target at all, just push back the texture to the list.
            // This ensures that the textures always retain the appropriate texture bind target and don't accidentally get bound to a different tex uniform.
            if(newTex != nullptr)
            {
                newTex->setTextureImageUnit(bindTarget);

                auto it = std::find(texturesInScene.begin(), texturesInScene.end(), newTex);
                if(it == texturesInScene.end())
                {
                    if(texturesInScene.empty())
                    {
                        texturesInScene.push_back(newTex);
                    }
                    else
                    {
                        texturesInScene.insert(texturesInScene.begin() + bindTarget, newTex);
                    }
                    
                }
                
                returnedTex = newTex;
            }
        }
    }
    ImGui::PopID();

    ImGui::SameLine();
    std::string unloadImgButtonID = "UnloadTexButton" + std::string(label);
    ImGui::PushID(unloadImgButtonID.c_str());
    // Unload tex button
    if(ImGui::Button("X"))
    {
        // Only unload the texture if the value isn't an empty Texture or the default missing texture.
        // It wouldn't make sense to delete an empty texture or an engine default
        if(value->getID() != 0 && value->getID() != missingImgTex.getID())
        {
            auto texIt = std::find(texturesInScene.begin(), texturesInScene.end(), value);
            
            // Remove the texture from the textures in the scene
            if(texIt != texturesInScene.end())
            {
                *texIt = nullptr;
                texturesInScene.erase(texIt);
            }

            // Find the name of the texture in the list of all loaded textures and unload it entirely
            std::string texKey;
            for(const auto &entry: loadedTextures)
            {
                if(entry.second == value)
                {
                    texKey = entry.first;
                    break;
                }
            }
            
            ResourceManager::getInstance().UnloadTexture(texKey);

            returnedTex = const_cast<Texture*>(&missingTex);
        }
    }
    ImGui::PopID();

    // NOTE: A combo to pick already loaded textures like it is with the shaders

    return returnedTex;
}

#pragma endregion