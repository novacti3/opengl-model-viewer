#pragma once

#include <glm/vec2.hpp> 
#include <glm/vec3.hpp> 

#include <vector>
#include <array>

struct Vertex final
{
    glm::vec3 position;
    glm::vec2 uv;

    Vertex(): position(glm::vec3(0.0f)), uv(glm::vec2(0.0f)){}
    Vertex(glm::vec3 position = glm::vec3(0.0f), glm::vec2 uv = glm::vec2(0.0f))
    {
        this->position = position;
        this->uv = uv;
    }

    Vertex(const Vertex& other)
    {
        if(&other != this)
        {
            this->position = other.position;
            this->uv = other.uv;
        }
    }
    Vertex& operator=(const Vertex& other)
    {
        if(&other != this)
        {
            this->position = other.position;
            this->uv = other.uv;
        }
        return *this;
    }

    Vertex(Vertex&& other)
    {
        if(&other != this)
        {
            this->position = std::move(other.position);
            this->uv = std::move(other.uv);
        }
    }
    Vertex& operator=(Vertex&& other)
    {
        if(&other != this)
        {
            this->position = std::move(other.position);
            this->uv = std::move(other.uv);
        }
        return *this;
    }

    ~Vertex()
    {
        position = glm::vec3(0.0f);
        uv = glm::vec2(0.0f);
    }
};

class Model
{
   protected:
   unsigned int _VAO, _VBO, _EBO;
   std::vector<Vertex> _vertices;

   public:
   Model();
   Model(const std::vector<Vertex> &vertices);
   ~Model();
   Model(const Model &other);
   Model &operator=(const Model &other);
   Model(Model &&other);
   Model &operator=(Model &&other);

   public:
   inline const unsigned int &getVAO() const { return _VAO; }
   inline const unsigned int &getVBO() const { return _VBO; }
   inline const unsigned int &getEBO() const { return _EBO; }
   inline const std::vector<Vertex> &getVertices() const { return _vertices; }

   void Bind() const;
   void Unbind() const;
};