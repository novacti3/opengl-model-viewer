#include "shader_uniform.hpp"

#include "texture.hpp"

#include <cstring>

ShaderUniform::ShaderUniform(): _name(""), _type(ShaderUniformType::UNDEFINED), value(nullptr) {}
ShaderUniform::ShaderUniform(const std::string name, const ShaderUniformType type, void* value) 
    : _name(name), _type(type) 
{
    this->value = value;
}
ShaderUniform::~ShaderUniform()
{
    this->_name.clear();
    this->_type = ShaderUniformType::UNDEFINED;
    DeleteValuePtr();
}
// Copy
ShaderUniform::ShaderUniform(const ShaderUniform& other)
{
    if(&other != this)
    {
        this->_name = other._name;
        this->_type = other._type;
        DeleteValuePtr();
        CopyValuePtr(other.value);
    }
}
ShaderUniform& ShaderUniform::operator=(ShaderUniform other)
{
    if(&other != this)
    {
        this->_name = other._name;
        this->_type = other._type;
        // FIXME: memcpy this shit
        DeleteValuePtr();
        CopyValuePtr(other.value);
    }
    return *this;
}
// Move
ShaderUniform::ShaderUniform(ShaderUniform&& other)
{
    if(&other != this)
    {
        this->_name = std::move(other._name);
        this->_type = std::move(other._type);
        
        DeleteValuePtr();
        this->value = other.value;
        other.value = nullptr;
    }
}
ShaderUniform& ShaderUniform::operator=(ShaderUniform&& other)
{
    if(&other != this)
    {
        this->_name = std::move(other._name);
        this->_type = std::move(other._type);
        
        DeleteValuePtr();
        this->value = other.value;
        other.value = nullptr;    
    }
    return *this;
}

void ShaderUniform::CopyValuePtr(void* const src)
{
    switch(_type)
    {
        case ShaderUniformType::INT:
            memcpy(this->value, src, sizeof(int));
        break;

        case ShaderUniformType::UINT:
            memcpy(this->value, src, sizeof(unsigned int));
        break; 

        case ShaderUniformType::BOOL:
            memcpy(this->value, src, sizeof(bool));
        break;

        case ShaderUniformType::FLOAT:
            memcpy(this->value, src, sizeof(float));
        break;
        case ShaderUniformType::VEC2:
            memcpy(this->value, src, 2 * sizeof(float));
        break;
        case ShaderUniformType::VEC3:
            memcpy(this->value, src, 3 * sizeof(float));
        break;
        case ShaderUniformType::VEC4:
            memcpy(this->value, src, 4 * sizeof(float));
        break;

        case ShaderUniformType::MAT2:
            memcpy(this->value, src, 2 * 2 * sizeof(float));
        break;

        case ShaderUniformType::MAT3:
            memcpy(this->value, src, 3 * 3 * sizeof(float));
        break;

        case ShaderUniformType::MAT4:
            memcpy(this->value, src, 4 * 4 * sizeof(float));
        break;

        case ShaderUniformType::TEX2D:
            memcpy(this->value, src, sizeof(Texture));
        break;
    }
}
void ShaderUniform::DeleteValuePtr()
{
    switch(_type)
    {
        case ShaderUniformType::INT:
            delete((int*)this->value);
        break;

        case ShaderUniformType::UINT:
            delete((unsigned int*)this->value);
        break; 

        case ShaderUniformType::BOOL:
            delete((bool*)this->value);
        break;

        case ShaderUniformType::FLOAT:
        case ShaderUniformType::VEC2:
        case ShaderUniformType::VEC3:
        case ShaderUniformType::VEC4:
        case ShaderUniformType::MAT2:
        case ShaderUniformType::MAT3:
        case ShaderUniformType::MAT4:
            delete((float*)this->value);
        break;

        case ShaderUniformType::TEX2D:
            delete((Texture*)this->value);
        break;
    }
}
