#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "tile.hpp"

extern const ivec2 directions[8];

struct Neighbors {
    bool any();
    bool up();
    bool right();
    bool down();
    bool left();
    bool up_right();
    bool down_right();
    bool down_left();
    bool up_left();

    static Neighbors check(std::vector<TILE> &grid, int wsize, ivec2 pos,
                           std::function<bool(TILE)> check);

  private:
    std::bitset<8> nbs;
};

bool in_range(ivec2 pos, int wsize);
