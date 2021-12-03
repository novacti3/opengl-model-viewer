#include "shader_uniform.hpp"

#include "texture.hpp"

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
// TODO: if (other != this)...
// Copy
ShaderUniform::ShaderUniform(const ShaderUniform& other)
{
    this->_name = other._name;
    this->_type = other._type;
    // FIXME: memcpy this shit
    DeleteValuePtr();
    this->value = other.value;
}
ShaderUniform& ShaderUniform::operator=(ShaderUniform other)
{
    this->_name = other._name;
    this->_type = other._type;
    // FIXME: memcpy this shit
    DeleteValuePtr();
    this->value = other.value;
    return *this;
}
// Move
ShaderUniform::ShaderUniform(ShaderUniform&& other)
{
    this->_name = std::move(other._name);
    this->_type = std::move(other._type);
    
    DeleteValuePtr();
    this->value = other.value;
    other.value = nullptr;
}
ShaderUniform& ShaderUniform::operator=(ShaderUniform&& other)
{
    this->_name = std::move(other._name);
    this->_type = std::move(other._type);
    
    DeleteValuePtr();
    this->value = other.value;
    other.value = nullptr;
    
    return *this;
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
