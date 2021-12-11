#pragma once

#include <glad/glad.h>

#include "shader_uniform.hpp"

#include <vector>

class Shader
{
    private:
    unsigned int _id = 0;
    std::vector<ShaderUniform*> _uniforms;

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
    inline const std::vector<ShaderUniform*> &getUniforms() const { return _uniforms; };
    inline const std::vector<ShaderUniform*> getUniformsOfType(const ShaderUniformType &type) const 
    {
        std::vector<ShaderUniform*> uniformsOfSpecifiedType;; 
        for(ShaderUniform* const uniform: _uniforms)
        {
            if(uniform->getType() == type)
                uniformsOfSpecifiedType.push_back(uniform);
        }
        return std::move(uniformsOfSpecifiedType);
    }

    void SetUniform(const std::string &name, const void* value);

    private:
    void UpdateUniforms() const;
    void CheckShaderForErrors(unsigned int shader);
    ShaderUniform* const ParseShaderUniformLine(const std::string &line);
};