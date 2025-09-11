#pragma once

#include "animations.hpp"
#include "lights.hpp"
#include "liquids.hpp"
#include "models.hpp"
#include "world.hpp"

struct Systems {
    Lights lights;
    Liquids liquids;
    World world;
    Models models;
    Animations animations;

    void init(Level &);
    void update();
    void render(Level &);
};
