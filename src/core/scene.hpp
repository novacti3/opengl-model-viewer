#pragma once

#include "misc/singleton.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "rendering/model.hpp"

#include <vector>

struct Scene final: public Singleton<Scene>
{
    friend class Singleton<Scene>;

    Model *model = nullptr;
    Shader *shader = nullptr;
    std::vector<Texture*> textures;

    private:
    Scene() = default;
    ~Scene()
    {
        model = nullptr;
        shader = nullptr;
        textures.clear();
    }
};