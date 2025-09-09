#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "base.hpp"
#include "liquids.hpp"
#include "world.hpp"

struct Level {
    usize width;
    usize total;

    Liquids liquids;
    World world;

    Level(int wsize) : width(wsize), total(wsize * wsize) {};

    // TODO:
    usize n_indices;

    void init(std::function<void(Level &l)> f);
    void update();
    void render();

    void add_liquid(LIQUID, ivec4);
};
