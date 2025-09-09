#pragma once

#include "base.hpp"
#include "indexed_verts.hpp"

enum struct LIQUID : u8 {
    WATER,
    OIL,
};

struct Liquid {
    ID id;
    LIQUID type;
    IndexedVerts verts;
    std::vector<float> original_ys;

    Liquid() = default;

    Liquid(LIQUID, ivec4 pos, int wsize);

    void update();
    void render();
};
