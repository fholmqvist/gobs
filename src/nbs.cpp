#include "nbs.hpp"

#include "ivec2.hpp"
#include "tile.hpp"

std::bitset<8> nbs_check(std::vector<TILE> &grid, int wsize, ivec2 pos,
                         std::function<bool(TILE)> check) {
    std::bitset<8> nbs;

    for (size i = 0; i < 8; i++) {
        auto nb = pos + (ivec2){ directions[i][0], directions[i][1] };
        if (!in_range(nb, wsize)) {
            continue;
        }
        if (check(grid[ivec2_to_idx(nb, wsize)])) {
            nbs.set(i, true);
        }
    }

    return nbs;
}

const ivec2 directions[8] = {
    // UP
    { 0, -1 },
    // RIGHT
    { 1, 0 },
    // DOWN
    { 0, 1 },
    // LEFT
    { -1, 0 },
    // UP RIGHT
    { 1, -1 },
    // DOWN RIGHT
    { 1, 1 },
    // DOWN LEFT
    { -1, 1 },
    // UP LEFT
    { -1, -1 }
};

bool in_range(ivec2 pos, int wsize) {
    return pos[0] >= 0 && pos[0] < wsize && pos[1] >= 0 && pos[1] < wsize;
}

bool nbs_up(std::bitset<8> nb) {
    return nb[0];
}

bool nbs_right(std::bitset<8> nb) {
    return nb[1];
}

bool nbs_down(std::bitset<8> nb) {
    return nb[2];
}

bool nbs_left(std::bitset<8> nb) {
    return nb[3];
}

bool nbs_up_right(std::bitset<8> nb) {
    return nb[4];
}

bool nbs_down_right(std::bitset<8> nb) {
    return nb[5];
}

bool nbs_down_left(std::bitset<8> nb) {
    return nb[6];
}

bool nbs_up_left(std::bitset<8> nb) {
    return nb[7];
}
