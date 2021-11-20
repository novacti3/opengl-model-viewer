#include "shader.hpp"

#include "core/log.hpp"

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
        // Not created by doing char name[nameBufferLength] because MSVC can't use non-const vars to create arrays on the stack
        char* name = new char[nameBufferLength];
        int size;
        unsigned int type;
        GL_CALL(glad_glGetActiveUniform(_id, i, sizeof(name), NULL, &size, &type, name));
        std::string nameStr(name);
        delete name;

        switch((ShaderUniformType)type)
        {
            case ShaderUniformType::VEC4:
            {
                float* value = new float[4];
                GL_CALL(glad_glGetUniformfv(_id, i, value));
                std::shared_ptr<ShaderUniform> uniform(new ShaderUniform(nameStr, (ShaderUniformType)type, (void*)value));     
                _uniforms.push_back(std::move(uniform));        
            } 
            break;
            
            case ShaderUniformType::MAT4:  
            {
                float* value = new float[4*4];
                GL_CALL(glad_glGetUniformfv(_id, i, value));
                std::shared_ptr<ShaderUniform> uniform(new ShaderUniform(nameStr, (ShaderUniformType)type, (void*)value));     
                _uniforms.push_back(std::move(uniform));        
            }
            break;
        }
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
    UpdateUniforms();
}

void Shader::Unbind() const
{
    GL_CALL(glad_glUseProgram(0));
}

void Shader::UpdateUniforms() const
{
    for(auto uniform: _uniforms)
    {
        switch(uniform.get()->getType())
        {
            case ShaderUniformType::VEC4:
            {
                unsigned int uniformLocation = GL_CALL(glad_glGetUniformLocation(_id, uniform.get()->getName().c_str()));
                GL_CALL(glad_glUniform4fv(uniformLocation, 1, (float*)(uniform.get()->value)));
            }
            break;
        }
    }
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