#include "model.hpp"

#include <glad/glad.h>

#include "../core/log.hpp"

Model::Model(): _VAO(0), _VBO(0) {}
Model::Model(std::vector<glm::vec3> vertices)
    : _vertices(vertices)
{
    GL_CALL(glad_glGenVertexArrays(1, &_VAO));
    GL_CALL(glad_glGenBuffers(1, &_VBO));

    GL_CALL(glad_glBindVertexArray(_VAO));
    
    GL_CALL(glad_glBindBuffer(GL_ARRAY_BUFFER, _VBO));
    GL_CALL(glad_glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STREAM_DRAW));

    GL_CALL(glad_glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), nullptr));
    GL_CALL(glad_glEnableVertexAttribArray(0));

    GL_CALL(glad_glBindVertexArray(0));
    GL_CALL(glad_glBindBuffer(GL_ARRAY_BUFFER, 0));
}
Model::~Model()
{
    GL_CALL(glad_glDeleteBuffers(1, &_VBO));
    GL_CALL(glad_glDeleteVertexArrays(1, &_VAO));
}