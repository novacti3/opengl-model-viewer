#include "texture.hpp"

#include "core/log.hpp"

#include <glad/glad.h>
#include <cstring>

Texture::Texture(): _id(0), _target(0), _imageUnit(0), _size(glm::vec2(0.0f)), _internalFormat(0), _format(0), data(nullptr) {}
Texture::Texture(int target, glm::uvec2 size, int internalFormat, int format, void* const data, int imageUnit)
    : _id(0), _target(target), _imageUnit(0), _size(size), _internalFormat(internalFormat), _format(format)
{
    this->data = const_cast<void*>(data);

    GL_CALL(glad_glGenTextures(1, &_id));
    Bind();
    GL_CALL(glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GL_CALL(glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GL_CALL(glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GL_CALL(glad_glTexImage2D(_target, 0, _internalFormat, _size.x, _size.y, 0, _format, GL_UNSIGNED_BYTE, data));
    Unbind();
}
Texture::~Texture()
{

    GL_CALL(glad_glDeleteTextures(1, &_id));
} 

// Copy
Texture::Texture(const Texture &other)
{
    memcpy(this->data, other.data, sizeof(other.data));
    this->_id             = other._id;
    this->_target         = other._target;
    this->_imageUnit      = other._imageUnit;
    this->_size           = other._size;
    this->_internalFormat = other._internalFormat;
    this->_format         = other._format;
}
Texture& Texture::operator=(Texture other)
{
    memcpy(this->data, other.data, sizeof(other.data));
    this->_id             = other._id;
    this->_target         = other._target;
    this->_imageUnit      = other._imageUnit;
    this->_size           = other._size;
    this->_internalFormat = other._internalFormat;
    this->_format         = other._format;

    return *this;
}

// Move
Texture::Texture(Texture&& other)
{
    this->data = other.data;
    other.data = nullptr;

    this->_id             = std::move(other._id);
    this->_target         = std::move(other._target);
    this->_imageUnit      = std::move(other._imageUnit);
    this->_size           = std::move(other._size);
    this->_internalFormat = std::move(other._internalFormat);
    this->_format         = std::move(other._format);
}
Texture& Texture::operator=(Texture&& other)
{
    this->data = other.data;
    other.data = nullptr;

    this->_id             = std::move(other._id);
    this->_target         = std::move(other._target);
    this->_imageUnit      = std::move(other._imageUnit);
    this->_size           = std::move(other._size);
    this->_internalFormat = std::move(other._internalFormat);
    this->_format         = std::move(other._format);
    
    return *this;
}

void Texture::Bind() const
{
    GL_CALL(glad_glBindTexture(_target, _id));
}
void Texture::Unbind() const
{
    GL_CALL(glad_glBindTexture(_target, 0));
}