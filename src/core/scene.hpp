#pragma once

#include "../misc/singleton.hpp"
#include "../rendering/shader.hpp"
#include "../rendering/texture.hpp"

#include <vector>

struct Scene: public Singleton<Scene>
{
    Shader *shader;
    std::vector<Texture*> textures;
};