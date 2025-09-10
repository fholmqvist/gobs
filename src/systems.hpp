#pragma once

#include "lights.hpp"
#include "liquids.hpp"
#include "models.hpp"
#include "world.hpp"

struct Systems {
    Lights lights;
    Liquids liquids;
    World world;
    Models models;

    void init(Level&);
    void update();
    void render(Level&);
};
