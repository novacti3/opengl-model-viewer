#pragma once

#include "misc/singleton.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"

#include <unordered_map>
#include <string>
#include <memory>

class ResourceManager final : public Singleton<ResourceManager>
{
    friend class Singleton<ResourceManager>;

    private:
    std::unordered_map<std::string, std::unique_ptr<Shader>> _loadedShaders;
    std::unordered_map<std::string, std::unique_ptr<Texture>> _loadedTextures;

    private:
    ResourceManager() = default;
    ~ResourceManager() = default;
    public:
    // Copy
    ResourceManager(const ResourceManager& other) = delete;
    ResourceManager& operator=(ResourceManager other) = delete;
    // Move
    ResourceManager(ResourceManager&& other) = delete;
    ResourceManager& operator=(ResourceManager&& other) = delete;

    Shader *CreateShaderFromFiles(const std::string &vertShaderPath, const std::string &fragShaderPath);
    const std::unique_ptr<Shader>* const GetShader(const std::string &name);
    void AddShader(Shader *shader, std::string name);

    Texture *CreateTextureFromFile(const std::string &path);
    const std::unique_ptr<Texture>* const GetTexture(const std::string &name);
    void AddTexture(Texture *texture, std::string name);
};