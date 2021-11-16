#include "shader.hpp"

#include "core/log.hpp"

ShaderUniform::ShaderUniform(const std::string name, const ShaderUniformType type) 
    : _name(name), _type(type) {}
ShaderUniform::~ShaderUniform()
{
    this->_name.clear();
    this->_type = ShaderUniformType::UNDEFINED;
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


Shader::Shader(const char *vertSource, const char *fragSource)
{
    unsigned int vertShader, fragShader;
    
    vertShader = GL_CALL(glad_glCreateShader(GL_VERTEX_SHADER));
    GL_CALL(glad_glShaderSource(vertShader, 1, &vertSource, 0));
    GL_CALL(glad_glCompileShader(vertShader));
    CheckShaderForErrors(vertShader);

    fragShader = GL_CALL(glad_glCreateShader(GL_FRAGMENT_SHADER));
    GL_CALL(glad_glShaderSource(fragShader, 1, &fragSource, 0));
    GL_CALL(glad_glCompileShader(fragShader));
    CheckShaderForErrors(fragShader);

    _id = GL_CALL(glad_glCreateProgram());
    GL_CALL(glad_glAttachShader(_id, vertShader));
    GL_CALL(glad_glAttachShader(_id, fragShader));
    GL_CALL(glad_glLinkProgram(_id));

    GL_CALL(glad_glDeleteShader(vertShader));
    GL_CALL(glad_glDeleteShader(fragShader));

    // Uniform parsing
    int numActiveUniforms;
    GL_CALL(glad_glGetProgramiv(_id, GL_ACTIVE_UNIFORMS, &numActiveUniforms));
    int nameBufferLength;
    GL_CALL(glad_glGetProgramiv(_id, GL_ACTIVE_UNIFORM_MAX_LENGTH, &nameBufferLength));

    for (int i = 0; i < numActiveUniforms; i++)
    {
        // NOTE: this can probably be squashed down
        char name[nameBufferLength];
        int size;
        unsigned int type;
        GL_CALL(glad_glGetActiveUniform(_id, i, sizeof(name), NULL, &size, &type, name));

        std::string nameStr(name);
        ShaderUniform uniform(nameStr, ShaderUniformType(type));
        _uniforms.push_back(uniform);
    }
}
Shader::~Shader()
{
    GL_CALL(glad_glDeleteProgram(_id));
}

// TODO: if (other != this)...
// Copy
Shader::Shader(const Shader& other)
{
    this->_id = other._id;
}
Shader& Shader::operator=(Shader other)
{
    this->_id = other._id;
    return *this;
}

// Move
Shader::Shader(Shader&& other)
{
    this->_id = std::move(other._id);
}
Shader& Shader::operator=(Shader&& other)
{
    this->_id = std::move(other._id);
    return *this;
}

void Shader::Bind() const
{
    GL_CALL(glad_glUseProgram(_id));
}

void Shader::Unbind() const
{
    GL_CALL(glad_glUseProgram(0));
}

void Shader::CheckShaderForErrors(unsigned int shader)
{
    int success;
    char infoLog[512];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        Log::LogError("Shader error: " + std::string(infoLog));
    }
}