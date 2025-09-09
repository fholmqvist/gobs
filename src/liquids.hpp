#pragma once

#include "liquid.hpp"
#include "world.hpp"

extern Shader liquid_shader;

struct Liquids {
    std::vector<Liquid> liquids;
    Shader shader;

    Liquids() : shader(liquid_shader) {};

    ID add(World &, LIQUID, ivec4);
    void update();
    void render();
};
