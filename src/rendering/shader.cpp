#include "shader.hpp"

#include "core/log.hpp"
#include "texture.hpp"

Shader::Shader(const char *vertSource, const char *fragSource): _id(0)
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

        void *value = nullptr;
        std::shared_ptr<ShaderUniform> uniform;
        switch((ShaderUniformType)type)
        {
            case ShaderUniformType::INT:
            case ShaderUniformType::BOOL:
                value = (void*)new int;
                GL_CALL(glad_glGetUniformiv(_id, i, (int*)value));
            break;
            case ShaderUniformType::UINT:
                value = (void*)new unsigned int;
                GL_CALL(glad_glGetUniformuiv(_id, i, (unsigned int*)value));
            break;
            case ShaderUniformType::FLOAT:
                value = (void*)new float;
                GL_CALL(glad_glGetUniformfv(_id, i, (float*)value));
            break;
            

            case ShaderUniformType::VEC2:
                value = (void*)new float[2];
                GL_CALL(glad_glGetUniformfv(_id, i, (float*)value));      
            break;
            case ShaderUniformType::VEC3:
                value = (void*)new float[3];
                GL_CALL(glad_glGetUniformfv(_id, i, (float*)value));     
            break;
            case ShaderUniformType::VEC4:
                value = (void*)new float[4];
                GL_CALL(glad_glGetUniformfv(_id, i, (float*)value));   
            break;
            

            case ShaderUniformType::MAT2:  
                value = (void*)new float[2*2];
                GL_CALL(glad_glGetUniformfv(_id, i, (float*)value));
            break;
            case ShaderUniformType::MAT3:  
                value = (void*)new float[3*3];
                GL_CALL(glad_glGetUniformfv(_id, i, (float*)value));   
            break;
            case ShaderUniformType::MAT4:  
                value = (void*)new float[4*4];
                GL_CALL(glad_glGetUniformfv(_id, i, (float*)value)); 
            break;


            case ShaderUniformType::TEX2D:
                value = (void*)new Texture;
            break;
        }
        uniform = std::make_shared<ShaderUniform>(nameStr, (ShaderUniformType)type, (void*)value);
        _uniforms.push_back(std::move(uniform));    
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
        unsigned int uniformLocation = GL_CALL(glad_glGetUniformLocation(_id, uniform.get()->getName().c_str()));
        switch(uniform.get()->getType())
        {
            case ShaderUniformType::INT:
            case ShaderUniformType::BOOL:
                GL_CALL(glad_glUniform1i(uniformLocation, *((int*)(uniform.get()->value))));
            break;
            case ShaderUniformType::UINT:
                GL_CALL(glad_glUniform1ui(uniformLocation, *((unsigned int*)(uniform.get()->value))));
            break;
            case ShaderUniformType::FLOAT:
                GL_CALL(glad_glUniform1f(uniformLocation, *((float*)(uniform.get()->value))));
            break;


            case ShaderUniformType::VEC2:
                GL_CALL(glad_glUniform2fv(uniformLocation, 1, (float*)(uniform.get()->value)));
            break;
            case ShaderUniformType::VEC3:
                GL_CALL(glad_glUniform3fv(uniformLocation, 1, (float*)(uniform.get()->value)));
            break;
            case ShaderUniformType::VEC4:
                GL_CALL(glad_glUniform4fv(uniformLocation, 1, (float*)(uniform.get()->value)));
            break;


            case ShaderUniformType::MAT2:
                GL_CALL(glad_glUniformMatrix2fv(uniformLocation, 1, false, (float*)(uniform.get()->value)));
            break;
            case ShaderUniformType::MAT3:
                GL_CALL(glad_glUniformMatrix3fv(uniformLocation, 1, false, (float*)(uniform.get()->value)));
            break;
            case ShaderUniformType::MAT4:
                GL_CALL(glad_glUniformMatrix4fv(uniformLocation, 1, false, (float*)(uniform.get()->value)));
            break;


            case ShaderUniformType::TEX2D:
                GL_CALL(glad_glUniform1i(uniformLocation, ( (Texture*)(uniform.get()->value) )->getID()));
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