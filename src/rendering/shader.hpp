#pragma once

#include <glad/glad.h>

#include <string>
#include <vector>

enum class ShaderUniformType
{
    UNDEFINED = 0,

    FLOAT = GL_FLOAT,
    INT = GL_INT,
    UINT = GL_UNSIGNED_INT,
    BOOL = GL_BOOL,
    
    VEC2 = GL_FLOAT_VEC2,
    VEC3 = GL_FLOAT_VEC3,
    VEC4 = GL_FLOAT_VEC4,

    MAT2 = GL_FLOAT_MAT2,
    MAT3 = GL_FLOAT_MAT3,
    MAT4 = GL_FLOAT_MAT4,

    TEX1D = GL_SAMPLER_1D,
    TEX2D = GL_SAMPLER_2D,
    TEX3D = GL_SAMPLER_3D
};

struct ShaderUniform
{
    private:
    std::string _name = "";
    ShaderUniformType _type = ShaderUniformType::UNDEFINED;
    // NOTE: Not optimal because it wastes memory for data types not containing 
    // 4 float values but I can't be arsed right now
    float _value[4];

    public: 
    ShaderUniform(const std::string name, const ShaderUniformType type, float* _value);
    // Copy
    ShaderUniform(const ShaderUniform& other);
    ShaderUniform& operator=(ShaderUniform other);
    // Move
    ShaderUniform(ShaderUniform&& other);
    ShaderUniform& operator=(ShaderUniform&& other);
    ~ShaderUniform();

    inline const std::string &getName()       const { return _name; }
    inline const ShaderUniformType &getType() const { return _type; }
    inline const float* const getValue() const { return _value; } 
};

class Shader
{
    private:
    unsigned int _id = 0;
    std::vector<ShaderUniform> _uniforms;

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
    inline const std::vector<ShaderUniform> &getUniforms() const { return _uniforms; }

    private:
    void CheckShaderForErrors(unsigned int shader);
};