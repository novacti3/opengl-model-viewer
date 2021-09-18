#pragma once

#include "misc/singleton.hpp"
#include "rendering/shader.hpp"

#include <unordered_map>
#include <string>

class ResourceManager final : public Singleton<ResourceManager>
{
    friend class Singleton<ResourceManager>;

    private:
    std::unordered_map<std::string, Shader*> _loadedShaders;

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

    public:
    void Cleanup();

    Shader *CreateShaderFromFiles(const std::string &vertShaderPath, const std::string &fragShaderPath);
    Shader *GetShader(const std::string &name);
    void AddShader(Shader *shader, std::string name);
};