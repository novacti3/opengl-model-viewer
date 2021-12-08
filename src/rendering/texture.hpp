#pragma once

#include <glm/vec2.hpp>

class Texture
{
    public:
    void *data;

    private:
    unsigned int _id;
    int _target;
    int _imageUnit;
    glm::uvec2 _size;
    int _internalFormat;
    int _format;
    
    public:
    // TODO: Adjustable tex params
    Texture();
    Texture(int target, glm::uvec2 size, int internalFormat, int format, void* const data = nullptr, int imageUnit = 0);
    ~Texture();
    // Copy
    Texture(const Texture &other);
    Texture& operator=(Texture other);
    // Move
    Texture(Texture&& other);
    Texture& operator=(Texture&& other);

    public:
    inline const unsigned int &getID()               const { return _id; }
    inline const int          &getTarget()           const { return _target; }
    inline const int          &getTextureImageUnit() const { return _imageUnit; }
    inline const glm::uvec2   &getSize()             const { return _size; }
    inline const int          &getInternalFormat()   const { return _internalFormat; }
    inline const int          &getFormat()           const { return _format; }

    inline void               setTextureImageUnit(int imageUnit) { _imageUnit = imageUnit; }

    void Bind() const;
    void Unbind() const;
};