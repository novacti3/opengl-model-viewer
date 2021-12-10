#include "renderer.hpp"

#include "core/log.hpp"
#include "core/resource_manager.hpp"

void Renderer::Init()
{
    std::vector<Vertex> cubeVertices;
    cubeVertices.insert(cubeVertices.end(), 
    {
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 0.0f)),

        Vertex(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),

        Vertex(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),

        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),

        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f, -0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f, -0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),

        Vertex(glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f, -0.5f),  glm::vec2(1.0f, 1.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3( 0.5f,  0.5f,  0.5f),  glm::vec2(1.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f,  0.5f,  0.5f),  glm::vec2(0.0f, 0.0f)),
        Vertex(glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec2(0.0f, 1.0f))
    });

    std::vector<Vertex> quadVertices;
    quadVertices.insert(quadVertices.end(),
    {
        Vertex(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2(0.0f, 1.0f)), // top left
        Vertex(glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f)), // bot left
        Vertex(glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f)), // bot right
        Vertex(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(1.0f, 1.0f)), // top right
    });

    _cube = new Model(std::move(cubeVertices));
    _quad = new Model(std::move(quadVertices));

    Scene::getInstance().model = _cube;
}

void Renderer::DeInit()
{
    delete _cube;
    delete _quad;
}

void Renderer::DrawScene()
{
    static const Shader &defaultShader = *(ResourceManager::getInstance().GetShader("default"));
    static const Texture &missingTex = *(ResourceManager::getInstance().GetTexture("tex_missing"));

    static Scene &scene = Scene::getInstance();
    auto &shaderUniforms = scene.shader->getUniforms();

    // FIXME: Throws error 1282 after just unloading a texture
    GL_CALL(glad_glEnable(GL_DEPTH_TEST));
    
    GL_CALL(glad_glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GL_CALL(glad_glClearColor(settings.bgColor.x, settings.bgColor.y, settings.bgColor.z, 1.0f));

    GL_CALL(glad_glPolygonMode(GL_FRONT_AND_BACK, (GLenum)settings.renderMode));

    if(!scene.textures.empty())
    {
        int numOfTextureShaderUniforms = 0;
        std::vector<int> textureUniformIndexes;
        for (int i = 0; i < shaderUniforms.size(); i++)
        {
            if(shaderUniforms[i]->getType() == ShaderUniformType::TEX2D)
            {
                textureUniformIndexes.push_back(i);
                numOfTextureShaderUniforms++;
            }
        }
        

        for (int i = 0; i < scene.textures.size() && i < numOfTextureShaderUniforms && i < 32; i++)
        {
            GL_CALL(glad_glActiveTexture(GL_TEXTURE0 + i));
            
            const Texture* const tex = (Texture*)(shaderUniforms[textureUniformIndexes.front()]->value);
            textureUniformIndexes.erase(textureUniformIndexes.begin());
            if(tex != nullptr && tex->getID() != 0)
                tex->Bind();
            else
                missingTex.Bind();
        }
    }
    else
    {
        GL_CALL(glad_glActiveTexture(GL_TEXTURE0));
        missingTex.Bind();
    }
        
    scene.model->Bind();
    
    if(scene.shader == nullptr)
        scene.shader = const_cast<Shader*>(&defaultShader);
    scene.shader->Bind();
    
    int numOfVerts = scene.model->getVertices().size();
    GL_CALL(glad_glDrawArrays(GL_TRIANGLES, 0, numOfVerts));
    // GL_CALL(glad_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
    
    scene.shader->Unbind();
    scene.model->Unbind();

    if(!scene.textures.empty())
    {
        int numOfTextureShaderUniforms = 0;
        std::vector<int> textureUniformIndexes;
        for (int i = 0; i < shaderUniforms.size(); i++)
        {
            if(shaderUniforms[i]->getType() == ShaderUniformType::TEX2D)
            {
                textureUniformIndexes.push_back(i);
                numOfTextureShaderUniforms++;
            }
        }
        

        for (int i = 0; i < scene.textures.size() && i < numOfTextureShaderUniforms && i < 32; i++)
        {
            GL_CALL(glad_glActiveTexture(GL_TEXTURE0 + i));
            
            const Texture* const tex = (Texture*)(shaderUniforms[textureUniformIndexes.front()]->value);
            textureUniformIndexes.erase(textureUniformIndexes.begin());
            if(tex != nullptr && tex->getID() != 0)
                tex->Unbind();
            else
                missingTex.Unbind();
        }
    }
    else
    {
        GL_CALL(glad_glActiveTexture(GL_TEXTURE0));
        missingTex.Unbind();
    }
};