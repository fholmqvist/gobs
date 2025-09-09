#pragma once

#include "liquid.hpp"

struct Liquids {
    std::vector<Liquid> liquids;

    Liquids() = default;

    ID add(LIQUID type, ivec4 pos, int wsize);
    void update();
    void render();
};
