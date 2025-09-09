#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "base.hpp"

struct Level {
    usize width;
    usize total;

    Level(int wsize) : width(wsize), total(wsize * wsize) {};

    // TODO:
    usize n_indices;

    void init(std::function<void(Level &l)> f);
    void update();
};
