#include "resource_manager.hpp"

#include "log.hpp"
#include "misc/utils.hpp"

#include <fstream>
#include <sstream>

#pragma region Shaders
Shader *ResourceManager::CreateShaderFromFiles(const std::string &vertShaderPath, const std::string &fragShaderPath)
{
    // NOTE: Might want to wrap this in a try-catch block
    std::ifstream vertShaderFile(vertShaderPath), fragShaderFile(fragShaderPath);
    std::stringstream vertShaderSourceStream, fragShaderSourceStream;
    std::string vertShaderSource, fragShaderSource;
    
    // Read the vertex shader file at the provided file path and store its contents
    if(!vertShaderFile.is_open())
    {
        Log::LogError("Couldn't open file " + vertShaderPath);
        return nullptr;
    }
    
    vertShaderSourceStream << vertShaderFile.rdbuf();
    vertShaderFile.close();
    
    // Read the fragment shader file at the provided file path and store its contents
    if(!fragShaderFile.is_open())
    {
        Log::LogError("Couldn't open file " + fragShaderPath);
        return nullptr;
    }

    fragShaderSourceStream << fragShaderFile.rdbuf();
    fragShaderFile.close();
    
    vertShaderSource = vertShaderSourceStream.str();
    fragShaderSource = fragShaderSourceStream.str();
    
    // Create a new shader from the extracted sources
    Shader *shader = new Shader(vertShaderSource.c_str(), fragShaderSource.c_str());
    return shader;
}

const std::unique_ptr<Shader>* const ResourceManager::GetShader(const std::string &name)
{
    for(const auto &shader: _loadedShaders)
    {
        if(shader.first.compare(name) == 0)
        {
            return &_loadedShaders[shader.first];
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

#pragma region Meshes
Mesh *ResourceManager::CreateMeshFromOBJFile(const std::string &path)
{
    std::ifstream objFile(path);
    std::stringstream dataStream;

    if(!objFile.is_open())
    {
        Log::LogError("Couldn't open file " + path);
        return nullptr;
    }

    dataStream << objFile.rdbuf();
    objFile.close();

    std::string line = "";
    std::vector<std::string> tokens;
    std::vector<glm::vec3> vertices;
    while(getline(dataStream, line))
    {
        // Ignore comments
        if(line[0] != '#')
        {
            tokens = SplitString(line, ' ');

            // Switch would make sense here but it can't be used with strings
            if(tokens[0] == "v")
            {
                float x = std::stof(tokens[1]);
                float y = std::stof(tokens[2]);
                float z = std::stof(tokens[3]);
                glm::vec3 vertex(x, y, z);
                vertices.push_back(std::move(vertex));
            }
            else if(tokens[0] == "vt")
            {
                // WIP
            }
            else if(tokens[0] == "f")
            {
                // WIP
            }

            tokens.clear();
        }
    }

    Mesh* mesh = new Mesh(std::move(vertices));
    return mesh;
}

const std::unique_ptr<Mesh>* const ResourceManager::GetMesh(const std::string &name)
{
    for(const auto &mesh: _loadedModels)
    {
        if(mesh.first.compare(name) == 0)
        {
            return &_loadedModels[mesh.first];
        }
    }
    Log::LogError("Couldn't find mesh " + name + " among loaded models");
    return nullptr;
}

void ResourceManager::AddMesh(Mesh *mesh, std::string name)
{
    std::unique_ptr<Mesh> smartPtr(mesh);
    _loadedModels.insert(std::make_pair(name, std::move(smartPtr)));
}
#pragma endregion