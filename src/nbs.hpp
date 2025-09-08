#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "tile.hpp"

extern const ivec2 directions[8];

std::bitset<8> nbs_check(std::vector<TILE> &grid, int wsize, ivec2 pos,
                         std::function<bool(TILE)> check);

bool in_range(ivec2 pos, int wsize);

bool nbs_up(std::bitset<8> nb);
bool nbs_right(std::bitset<8> nb);
bool nbs_down(std::bitset<8> nb);
bool nbs_left(std::bitset<8> nb);
bool nbs_up_right(std::bitset<8> nb);
bool nbs_down_right(std::bitset<8> nb);
bool nbs_down_left(std::bitset<8> nb);
bool nbs_up_left(std::bitset<8> nb);
