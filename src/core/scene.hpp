#pragma once

#include "../misc/singleton.hpp"
#include "../rendering/shader.hpp"
#include "../rendering/texture.hpp"

#include <vector>

struct Scene final: public Singleton<Scene>
{
    friend class Singleton<Scene>;

    Shader *shader = nullptr;
    std::vector<Texture*> textures;

    private:
    Scene() = default;
    ~Scene()
    {
        shader = nullptr;
        textures.clear();
    }
};