#pragma once

#include <glm/vec3.hpp>

#include <vector>

class Model
{
    private:
    unsigned int _VAO, _VBO;
    std::vector<glm::vec3> _vertices;

    public:
    Model();
    Model(std::vector<glm::vec3> vertices);
    ~Model();

    inline const unsigned int &getVAO() const { return _VAO; }
    inline const unsigned int &getVBO() const { return _VBO; }
    inline const std::vector<glm::vec3> &getVertices() const { return _vertices; }
};