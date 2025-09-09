#pragma once

#include "liquids.hpp"
#include "world.hpp"

struct Systems {
    World world;
    Level level;
    Liquids liquids;

    Systems() : level(-1) {};
};
