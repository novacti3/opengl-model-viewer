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
    // FIXME: This should copy the value of the pointer,
    // not just make this instance's pointer point to the other,
    // because when either instance gets changed, the other instance's
    // value changes also
    
    switch(this->_type)
    {
        case ShaderUniformType::VEC4:
        case ShaderUniformType::MAT4:
            delete((float*)this->value);
        break;
    }
    this->value = other.value;
}
ShaderUniform& ShaderUniform::operator=(ShaderUniform other)
{
    this->_name = other._name;
    this->_type = other._type;
    // FIXME: This should copy the value of the pointer,
    // not just make this instance's pointer point to the other,
    // because when either instance gets changed, the other instance's
    // value changes also
    delete(this->value);
    this->value = other.value;
    return *this;
}
// Move
ShaderUniform::ShaderUniform(ShaderUniform&& other)
{
    this->_name = std::move(other._name);
    this->_type = std::move(other._type);
    
    switch(this->_type)
    {
        case ShaderUniformType::VEC4:
        case ShaderUniformType::MAT4:
            delete((float*)this->value);
        break;
    }
    this->value = other.value;
    other.value = nullptr;
}
ShaderUniform& ShaderUniform::operator=(ShaderUniform&& other)
{
    this->_name = std::move(other._name);
    this->_type = std::move(other._type);
    
    switch(this->_type)
    {
        case ShaderUniformType::VEC4:
        case ShaderUniformType::MAT4:
            delete((float*)this->value);
        break;
    }
    this->value = other.value;
    other.value = nullptr;
    
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
