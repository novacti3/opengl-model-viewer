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
Shader* ResourceManager::CreateShaderFromFiles(const std::string &vertShaderPath, const std::string &fragShaderPath)
{
    std::string vertShaderSource = ReadFile(vertShaderPath);
    std::string fragShaderSource = ReadFile(fragShaderPath);

    Shader *shader = new Shader(vertShaderSource.c_str(), fragShaderSource.c_str());
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
    Log::LogError("Couldn't find shader " + name + " among loaded shaders");
    return nullptr;
}

void ResourceManager::AddShader(Shader *shader, std::string name)
{
    std::unique_ptr<Shader> smartPtr(shader);
    _loadedShaders.insert(std::make_pair(name, std::move(smartPtr)));
}
#pragma endregion

#pragma region Textures
Texture* ResourceManager::CreateTextureFromFile(const std::string &path)
{
    int width, height;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, nullptr, 0);
    Texture *tex = new Texture(GL_TEXTURE_2D, glm::vec2(width, height), GL_RGB, GL_RGB, (void*)data);
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
    Log::LogError("Couldn't find texture " + name + " among loaded textures");
    return nullptr;
}

void ResourceManager::AddTexture(Texture *texture, std::string name)
{
    std::unique_ptr<Texture> smartPtr(texture);
    _loadedTextures.insert(std::make_pair(name, std::move(smartPtr)));
}
#pragma endregion