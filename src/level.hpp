#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "base.hpp"
#include "systems.hpp"

struct Level {
    usize width;
    usize total;

    std::function<void(Level &l)> initf;

    Level(int wsize, std::function<void(Level &)> _initf)
        : width(wsize), total(wsize * wsize), initf(_initf) {};

    Systems systems;

    // TODO:
    usize n_indices;

    void init();
    void update();
    void render();

    void add_liquid(LIQUID, ivec4);
};
