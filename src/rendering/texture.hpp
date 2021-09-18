#pragma once

#include <glm/vec2.hpp>

class Texture
{
    public:
    void *data;

    private:
    unsigned int _id;
    int _target;
    glm::uvec2 _size;
    int _internalFormat;
    int _format;
    
    public:
    // TODO: Adjustable tex params
    Texture(int target, glm::uvec2 size, int internalFormat, int format, const void *data = nullptr);
    ~Texture();
    // Copy
    Texture(const Texture &other);
    Texture& operator=(Texture other);
    // Move
    Texture(Texture&& other);
    Texture& operator=(Texture&& other);

    public:
    inline const unsigned int &getID()             const { return _id; }
    inline const int          &getTarget()         const { return _target; }
    inline const glm::uvec2   &getSize()           const { return _size; }
    inline const int          &getInternalFormat() const { return _internalFormat; }
    inline const int          &getFormat()         const { return _format; }

    void Bind() const;
    void Unbind() const;
};