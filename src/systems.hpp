#pragma once

#include "lights.hpp"
#include "liquids.hpp"
#include "world.hpp"

struct Systems {
    Lights lights;
    Liquids liquids;
    World world;

    void init(Level&);
};
