#pragma once

#include "liquid.hpp"
#include "shader.hpp"

extern Shader liquid_shader;

struct Liquids {
    std::vector<Liquid> liquids;
    Shader shader;

    Liquids() : shader(liquid_shader) {};

    void init();
    ID add(LIQUID type, ivec4 pos, int wsize);
    void update();
    void render(Level& l);
};
