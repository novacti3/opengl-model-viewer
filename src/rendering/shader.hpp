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
    inline const std::vector<std::shared_ptr<ShaderUniform>> &getUniforms() const { return _uniforms; }

    private:
    void CheckShaderForErrors(unsigned int shader);
};