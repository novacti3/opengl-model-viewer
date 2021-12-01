#pragma once

#include <glm/vec3.hpp>

#include <vector>

class Mesh
{
    private:
    unsigned int _VAO, _VBO;
    std::vector<glm::vec3> _vertices;

    public:
    Mesh();
    Mesh(std::vector<glm::vec3> vertices);
    ~Mesh();

    inline const unsigned int &getVAO() const { return _VAO; }
    inline const unsigned int &getVBO() const { return _VBO; }
    inline const std::vector<glm::vec3> &getVertices() const { return _vertices; }
};