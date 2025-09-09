#pragma once

#include "base.hpp"
#include "pch.hpp" // IWYU pragma: keep

struct Level {
    usize width;
    usize total;

    Level(int wsize) : width(wsize), total(wsize * wsize) {};

    // TODO:
    usize n_indices;

    void init(std::function<void(Level &l)> f);
    void update();
};
