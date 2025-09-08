#pragma once

#include "base.hpp"
#include "pch.hpp" // IWYU pragma: keep

struct Level {
    usize width;
    usize total;

    Level() = default;

    static Level from_file(std::string filename);

    void init(std::function<void(Level &l)> f);
    void update();
    void render();
};
