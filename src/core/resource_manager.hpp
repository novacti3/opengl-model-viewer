#pragma once

#include "misc/singleton.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"

#include <unordered_map>
#include <string>
#include <memory>
#include <utility>

using LoadedShadersMap = std::unordered_map<std::string, std::unique_ptr<Shader>>;
using LoadedTexturesMap = std::unordered_map<std::string, std::unique_ptr<Texture>>;

class ResourceManager final : public Singleton<ResourceManager>
{
    friend class Singleton<ResourceManager>;

    private:
    LoadedShadersMap _loadedShaders;
    LoadedTexturesMap _loadedTextures;

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


    inline const LoadedShadersMap &getLoadedShaders()   { return _loadedShaders;  }
    inline const LoadedTexturesMap &getLoadedTextures() { return _loadedTextures; }

    static std::string ReadFile(const std::string &path);
    static std::pair<std::string, std::string> ParseFileNameAndExtension(const std::string &path);

    Shader *LoadShaderFromFiles(const std::string &vertShaderPath, const std::string &fragShaderPath);
    const Shader* const GetShader(const std::string &name);
    void AddLoadedShader(Shader *shader, std::string name);
    void UnloadShader(const std::string &name);

    Texture *LoadTextureFromFile(const std::string &path);
    const Texture* const GetTexture(const std::string &name);
    void AddLoadedTexture(Texture *texture, std::string name);
    void UnloadTexture(const std::string &name);
};