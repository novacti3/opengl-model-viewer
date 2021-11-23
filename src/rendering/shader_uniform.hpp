#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

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
    public:
    void* value = nullptr;

    public: 
    ShaderUniform();
    ShaderUniform(const std::string name, const ShaderUniformType type, void* value);
    // Copy
    ShaderUniform(const ShaderUniform& other);
    ShaderUniform& operator=(ShaderUniform other);
    // Move
    ShaderUniform(ShaderUniform&& other);
    ShaderUniform& operator=(ShaderUniform&& other);
    ~ShaderUniform();

    inline const std::string &getName()       const { return _name; }
    inline const ShaderUniformType &getType() const { return _type; }

    private:
    void DeleteValuePtr();
};
