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
        // Middleman char buffer here because doing std::string.data()
        // doesn't fill the string with data for some reason
        char name[nameBufferLength];
        int size;
        unsigned int type;
        GL_CALL(glad_glGetActiveUniform(_id, i, sizeof(name), NULL, &size, &type, name));
        std::string nameStr(name);

        switch((ShaderUniformType)type)
        {
            case ShaderUniformType::VEC4:
            case ShaderUniformType::MAT4:
            {
                // The same address gets assigned to the local var in all loop iterations
                float* value = (float*)malloc(4 * sizeof(float));
                GL_CALL(glad_glGetUniformfv(_id, i, value));
                // FIXME: Crashes the program after the first uniform
                // SIGABRT
                // raise.c Could not load source './signal/../sysdeps/unix/sysv/linux/raise.c': 
                std::shared_ptr<ShaderUniform> uniform(new ShaderUniform(name, ShaderUniformType(type), (void*)value));
                _uniforms.push_back(std::move(uniform));
                value = nullptr;
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