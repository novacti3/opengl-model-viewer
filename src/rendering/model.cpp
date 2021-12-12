#include "model.hpp"

#include <glad/glad.h>

#include "core/log.hpp"

Model::Model()
    : _VAO(0), _VBO(0), _EBO(0){}
Model::Model(const std::vector<Vertex> &vertices)
    : _vertices(vertices)
{
    GL_CALL(glad_glGenVertexArrays(1, &_VAO));
    GL_CALL(glad_glGenBuffers(1, &_VBO));
    GL_CALL(glad_glGenBuffers(1, &_EBO));

    GL_CALL(glad_glBindVertexArray(_VAO));

    GL_CALL(glad_glBindBuffer(GL_ARRAY_BUFFER, _VBO));
    // The size of the data must be written out like this because just doing _vertices.size()
    // returns the amount of elements rather than the size of the data itself 
    GL_CALL(glad_glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * _vertices.size(), (void*)_vertices.data(), GL_STATIC_DRAW));

    // GL_CALL(glad_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
    // GL_CALL(glad_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

    /*
                        Vertex format:
            Position     Tex coords       Normal
        vx   vy   vz   \   u   v   \   nx   ny   nz
    */
    // Vertex position
    GL_CALL(glad_glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(glm::vec3) + sizeof(glm::vec2) + sizeof(glm::vec3), (void*)0));
    GL_CALL(glad_glEnableVertexAttribArray(0));
    // UV coords
    GL_CALL(glad_glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(glm::vec2) + sizeof(glm::vec3) + sizeof(glm::vec3), (void*)(sizeof(glm::vec3))));
    GL_CALL(glad_glEnableVertexAttribArray(1));
    // Normals
    GL_CALL(glad_glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2), (void*)(sizeof(glm::vec3) + sizeof(glm::vec2))));
    GL_CALL(glad_glEnableVertexAttribArray(2));


    GL_CALL(glad_glBindVertexArray(0));
    GL_CALL(glad_glBindBuffer(GL_ARRAY_BUFFER, 0));
}
Model::~Model()
{
    GL_CALL(glad_glBindVertexArray(0));

    GL_CALL(glad_glDeleteBuffers(1, &_EBO));
    GL_CALL(glad_glDeleteBuffers(1, &_VBO));
    GL_CALL(glad_glDeleteVertexArrays(1, &_VAO));
}
Model::Model(const Model &other)
{
    if(&other != this)
    {
        this->_VAO = other._VAO;
        this->_VBO = other._VBO;
        this->_EBO = other._EBO;
        this->_vertices = other._vertices;
    }
}
Model &Model::operator=(const Model &other)
{
    if(&other != this)
    {
        this->_VAO = other._VAO;
        this->_VBO = other._VBO;
        this->_EBO = other._EBO;
        this->_vertices = other._vertices;
    }
    return *this;
}
Model::Model(Model &&other)
{
    if(&other != this)
    {
        this->_VAO = std::move(other._VAO);
        this->_VBO = std::move(other._VBO);
        this->_EBO = std::move(other._EBO);
        this->_vertices = std::move(other._vertices);
    }
}
Model &Model::operator=(Model &&other)
{
    if(&other != this)
    {
        this->_VAO = std::move(other._VAO);
        this->_VBO = std::move(other._VBO);
        this->_EBO = std::move(other._EBO);
        this->_vertices = std::move(other._vertices);
    }
    return *this;
}


void Model::Bind() const
{
    GL_CALL(glad_glBindVertexArray(_VAO));
}
void Model::Unbind() const
{
    GL_CALL(glad_glBindVertexArray(0));
}