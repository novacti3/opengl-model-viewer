#include "resource_manager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "log.hpp"
#include "misc/utils.hpp"

#include <fstream>
#include <sstream>

std::string ResourceManager::ReadFile(const std::string &path)
{
    try
    {
        std::ifstream fileStream(path);
        if(!fileStream.is_open())
        {
            Log::LogError("Couldn't read file, path: " + path);
            return "";
        }

        std::stringstream stringStream;
        stringStream << fileStream.rdbuf();
        fileStream.close();

        std::string contents = stringStream.str();
        return contents;
    }
    catch(const std::exception& e)
    {
        // NOTE: This might be redundant because of the is_open check in the try block
        Log::LogError(e.what());
        return "";
    }
}

#pragma region Shaders
Shader* ResourceManager::LoadShaderFromFiles(const std::string &vertShaderPath, const std::string &fragShaderPath)
{
    // Get rid of the file extension and get the name of the shader
    std::vector<std::string> splitVertPath = SplitString(vertShaderPath, '/');
    std::string shaderName = SplitString(splitVertPath[splitVertPath.size() - 1], '.')[0];

    if(GetShader(shaderName) != nullptr)
    {
        Log::LogWarning("Stopped loading shader '" + shaderName + "' because it's been loaded already");
        return const_cast<Shader*>(GetShader(shaderName));
    }

    std::string vertShaderSource = ReadFile(vertShaderPath);
    std::string fragShaderSource = ReadFile(fragShaderPath);

    Shader *shader = new Shader(vertShaderSource.c_str(), fragShaderSource.c_str());
    AddLoadedShader(shader, shaderName);
    Log::LogInfo("Loaded new shader, name: '" + shaderName + "'");
    return shader;
}

const Shader* const ResourceManager::GetShader(const std::string &name)
{
    for(const auto &shader: _loadedShaders)
    {
        if(shader.first.compare(name) == 0)
        {
            return _loadedShaders[shader.first].get();
        }
    }
    Log::LogWarning("Couldn't find shader '" + name + "' among loaded shaders");
    return nullptr;
}

void ResourceManager::AddLoadedShader(Shader *shader, std::string name)
{
    if(shader != nullptr)
    {
        std::unique_ptr<Shader> smartPtr(shader);
        _loadedShaders.insert(std::make_pair(name, std::move(smartPtr)));
    }
}
void ResourceManager::UnloadShader(const std::string &name)
{
    for(const auto &shader: _loadedShaders)
    {
        if(shader.first.compare(name) == 0)
        {
            shader.second.get()->Unbind();
            // delete shader.second.get();
            _loadedShaders.erase(shader.first);
            Log::LogInfo("Unloaded shader '" + name + "'");
            return;
        }
    }

    Log::LogInfo("Failed unloading shader '" + name +"', shader not among loaded shaders");
}
#pragma endregion

#pragma region Textures
Texture* ResourceManager::LoadTextureFromFile(const std::string &path)
{
    std::vector<std::string> splitPath = SplitString(path, '/');
    std::string texName = SplitString(splitPath[splitPath.size() - 1], '.')[0];

    if(GetTexture(texName) != nullptr)
    {
        Log::LogWarning("Stopped loading texture '" + texName + "' because it's been loaded already");
        return const_cast<Texture*>(GetTexture(texName));
    }

    int width, height;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, nullptr, 0);
    Texture *tex = new Texture(GL_TEXTURE_2D, glm::vec2(width, height), GL_RGB, GL_RGB, (void*)data);
    
    AddLoadedTexture(tex, texName);
    Log::LogInfo("Loaded new texture '" + texName + "'");
    return tex;
}

const Texture* const ResourceManager::GetTexture(const std::string &name)
{
    for(const auto &tex: _loadedTextures)
    {
        if(tex.first.compare(name) == 0)
        {
            return _loadedTextures[tex.first].get();
        }
    }
    Log::LogWarning("Couldn't find texture '" + name + "' among loaded textures");
    return nullptr;
}

void ResourceManager::AddLoadedTexture(Texture *texture, std::string name)
{
    if(texture != nullptr)
    {
        std::unique_ptr<Texture> smartPtr(texture);
        _loadedTextures.insert(std::make_pair(name, std::move(smartPtr)));
    }
}

void ResourceManager::UnloadTexture(const std::string &name)
{
    for(const auto &tex: _loadedTextures)
    {
        if(tex.first.compare(name) == 0)
        {
            // delete tex.second.get();
            _loadedTextures.erase(name);
            Log::LogInfo("Unloaded texture '" + name + "'");
            return;
        }
    }

    Log::LogInfo("Failed unloading texture '" + name +"', texture not among loaded textures");
}
#pragma endregion