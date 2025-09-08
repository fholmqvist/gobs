#include "neighbors.hpp"

#include "ivec2.hpp"
#include "tile.hpp"

Neighbors Neighbors::check(std::vector<TILE> &grid, int wsize, ivec2 pos,
                           std::function<bool(TILE)> check) {
    Neighbors nbs;

    for (size i = 0; i < 8; i++) {
        auto nb = pos + (ivec2){ directions[i][0], directions[i][1] };
        if (!in_range(nb, wsize)) {
            continue;
        }
        if (check(grid[ivec2_to_idx(nb, wsize)])) {
            nbs.nbs.set(i, true);
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

bool Neighbors::any() {
    return nbs.any();
}

bool Neighbors::up() {
    return nbs[0];
}

bool Neighbors::right() {
    return nbs[1];
}

bool Neighbors::down() {
    return nbs[2];
}

bool Neighbors::left() {
    return nbs[3];
}

bool Neighbors::up_right() {
    return nbs[4];
}

bool Neighbors::down_right() {
    return nbs[5];
}

bool Neighbors::down_left() {
    return nbs[6];
}

bool Neighbors::up_left() {
    return nbs[7];
}
