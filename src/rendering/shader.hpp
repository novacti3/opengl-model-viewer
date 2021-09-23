#pragma once

class Shader
{
    private:
    unsigned int _id;

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

    private:
    void CheckShaderForErrors(unsigned int shader);
};