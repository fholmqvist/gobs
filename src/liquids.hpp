#pragma once

#include "liquid.hpp"
#include "shader.hpp"
#include "storage.hpp"

extern Shader liquid_shader;

struct Liquids {
    Storage<Liquid> liquids;
    Shader shader;

    Liquids() : shader(liquid_shader) {};

    void init();
    GID add(LIQUID type, ivec4 pos, int wsize);
    void update();
    void render(Level &l);
};
