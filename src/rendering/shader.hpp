#pragma once

#include <glad/glad.h>

#include "shader_uniform.hpp"

#include <vector>
#include <memory>

class Shader
{
    private:
    unsigned int _id = 0;
    std::vector<std::shared_ptr<ShaderUniform>> _uniforms;

    public:
    Shader(const char *vertSource, const char *fragSource);
    // Copy
    Shader(const Shader& other);
    Shader& operator=(Shader other);
    // Move
    Shader(Shader&& other);
    Shader& operator=(Shader&& other);
    ~Shader();

    public:
    void Bind() const;
    void Unbind() const;

    inline const unsigned int &getID() const { return _id; }
    inline const std::vector<ShaderUniform*> getUniforms() const 
    {
        std::vector<ShaderUniform*> rawUniforms;
        for(const auto &smartPtr: _uniforms)
        {
            rawUniforms.push_back(smartPtr.get());
        }
        return rawUniforms;
    };

    void SetUniform(const std::string &name, const void* value)
    {
        for(auto uniform: _uniforms)
        {
            if(uniform.get()->getName() == name)
            {
                uniform.get()->value = const_cast<void*>(value);
            }
        }
    }

    private:
    void UpdateUniforms() const;
    void CheckShaderForErrors(unsigned int shader);
};