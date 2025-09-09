#pragma once

#include "liquid.hpp"
#include "world.hpp"

struct Liquids {
    std::vector<Liquid> liquids;

    Liquids() = default;

    ID add(World&, LIQUID, ivec4);
    void update();
    void render();
};
