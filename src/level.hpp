#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "base.hpp"
#include "systems.hpp"

struct Level {
    usize width;
    usize total;

    Level(int wsize) : width(wsize), total(wsize * wsize) {};

    Systems systems;

    // TODO:
    usize n_indices;

    void init(std::function<void(Level &l)> f);
    void update();
    void render();

    void add_liquid(LIQUID, ivec4);
};
