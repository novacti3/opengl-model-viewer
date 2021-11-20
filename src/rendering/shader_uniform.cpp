#include "shader_uniform.hpp"

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
}
ShaderUniform& ShaderUniform::operator=(ShaderUniform other)
{
    this->_name = other._name;
    this->_type = other._type;
    return *this;
}
// Move
ShaderUniform::ShaderUniform(ShaderUniform&& other)
{
    this->_name = std::move(other._name);
    this->_type = std::move(other._type);
}
ShaderUniform& ShaderUniform::operator=(ShaderUniform&& other)
{
    this->_name = std::move(other._name);
    this->_type = std::move(other._type);
    return *this;
}

void ShaderUniform::DeleteValuePtr()
{
    switch(_type)
    {
        case ShaderUniformType::VEC4:
        case ShaderUniformType::MAT4:
            delete((float*)value);
        break;
    }
}