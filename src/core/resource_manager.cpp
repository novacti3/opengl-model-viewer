#include "resource_manager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

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
std::pair<std::string, std::string> ResourceManager::ParseFileNameAndExtension(const std::string &path)
{
    std::vector<std::string> splitPath = SplitString(path, '/');
    std::vector<std::string> fileNameWithExtension = SplitString(splitPath[splitPath.size() - 1], '.');
    std::string fileName = fileNameWithExtension[0];
    std::string extension = fileNameWithExtension[1];

    return make_pair(fileName, extension);
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
            return _loadedShaders[shader.first];
        }
    }
    Log::LogWarning("Couldn't find shader '" + name + "' among loaded shaders");
    return nullptr;
}

void ResourceManager::AddLoadedShader(Shader *shader, std::string name)
{
    if(shader != nullptr)
    {
        _loadedShaders.insert(std::make_pair(name, shader));
    }
}
void ResourceManager::UnloadShader(const std::string &name)
{
    for(const auto &shader: _loadedShaders)
    {
        if(shader.first.compare(name) == 0)
        {
            shader.second->Unbind();
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
    
    auto fileNameAndExtension = ParseFileNameAndExtension(path);
    if(fileNameAndExtension.second.compare("jpg") != 0 && fileNameAndExtension.second.compare("png") != 0)
    {
        Log::LogError("Texture loading failed, please provide a file of an image file type (JPEG, PNG)\n Provided file: " + path);
        return nullptr;
    }

    if(GetTexture(fileNameAndExtension.first) != nullptr)
    {
        Log::LogWarning("Stopped loading texture '" + fileNameAndExtension.first + "' because it's been loaded already");
        return const_cast<Texture*>(GetTexture(fileNameAndExtension.first));
    }

    int width, height;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, nullptr, 0);
    Texture *tex = new Texture(GL_TEXTURE_2D, glm::vec2(width, height), GL_RGB, GL_RGB, (void*)data);
    
    AddLoadedTexture(tex, fileNameAndExtension.first);
    Log::LogInfo("Loaded new texture '" + fileNameAndExtension.first + "'");
    return tex;
}

const Texture* const ResourceManager::GetTexture(const std::string &name)
{
    for(const auto &tex: _loadedTextures)
    {
        if(tex.first.compare(name) == 0)
        {
            return _loadedTextures[tex.first];
        }
    }
    Log::LogWarning("Couldn't find texture '" + name + "' among loaded textures");
    return nullptr;
}

void ResourceManager::AddLoadedTexture(Texture *texture, std::string name)
{
    if(texture != nullptr)
    {
        _loadedTextures.insert(std::make_pair(name, texture));
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

#pragma region Models
Model *ResourceManager::LoadModelFromOBJFile(const std::string &path)
{
    std::string objFileContents = ReadFile(path);
    
    tinyobj::ObjReaderConfig config;
    config.mtl_search_path = "";
    config.triangulate = true;
    config.vertex_color = false;
    
    tinyobj::ObjReader reader;
    reader.ParseFromString(objFileContents, "", config);

    if(reader.Valid())
    {
        auto &attrib = reader.GetAttrib();
        auto &shapes = reader.GetShapes();

        std::vector<Vertex> vertices;

        // Loop through each shape
        for(const auto &shape: shapes)
        {
            // Loop through all of the indices of the given shape to construct Vertices
            for(const auto &index: shape.mesh.indices)
            {
                glm::vec3 pos(0.0f);
                // 3 * index is here because each vertex has 3 position coordinates
                // Acts basically the same way as the stride for OpenGL vert attrib ptrs
                {
                    float x = attrib.vertices[(3 * index.vertex_index) + 0];
                    float y = attrib.vertices[(3 * index.vertex_index) + 1];
                    float z = attrib.vertices[(3 * index.vertex_index) + 2];
                    pos = glm::vec3(x, y, z);
                }

                glm::vec2 uv(0.0f);
                // Only include UV coordinates if they are present
                if(index.texcoord_index >= 0)
                {
                    // The OBJ file format uses the coordinate system of 0 being the bottom of the image.
                    // OpenGL uses a system where 1 is the bottom of the image, therefore the
                    // vertical UV coordinate must be flipped
                    float u = attrib.texcoords[(2 * index.texcoord_index) + 0];
                    float v = 1.0f - attrib.texcoords[(2 * index.texcoord_index) + 1];
                    uv = glm::vec2(u, v);
                }

                glm::vec3 normal(0.0f);
                if(index.normal_index >= 0)
                {
                    float x = attrib.normals[(3 * index.normal_index) + 0];
                    float y = attrib.normals[(3 * index.normal_index) + 1]; 
                    float z = attrib.normals[(3 * index.normal_index) + 2]; 
                    normal = glm::vec3(x, y, z);
                }

                Vertex newVert(pos, uv, normal);
                vertices.push_back(std::move(newVert));
            }
        }

        std::string name = ParseFileNameAndExtension(path).first;
        Model *model = new Model(std::move(vertices));
        AddLoadedModel(model, name);
        return model;
    }
    else
    {
        Log::LogError(reader.Error());
        return nullptr;
    }
}
const Model* const ResourceManager::GetModel(const std::string &name)
{
    for(const auto &model: _loadedModels)
        {
            if(model.first.compare(name) == 0)
            {
                return _loadedModels[model.first];
            }
        }
        Log::LogWarning("Couldn't find model '" + name + "' among loaded models");
        return nullptr;
}
void ResourceManager::AddLoadedModel(Model *model, std::string name)
{
    if(model != nullptr)
    {
        _loadedModels.insert(std::make_pair(name, model));
    }
}
void ResourceManager::UnloadModel(const std::string &name)
{
    for(const auto &model: _loadedModels)
    {
        if(model.first.compare(name) == 0)
        {
            _loadedModels.erase(name);
            Log::LogInfo("Unloaded model '" + name + "'");
            return;
        }
    }

    Log::LogInfo("Failed unloading model '" + name +"', model not among loaded models");
}
#pragma endregion