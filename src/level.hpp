#pragma once

#include "pch.hpp" // IWYU pragma: keep

struct Level {
    Level() = default;

    static Level from_file(std::string filename);

    void init(std::function<void(Level &l)> f);
    void update();
    void render();
};
