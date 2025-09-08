#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "indexed_verts.hpp"
#include "shader.hpp"
#include "tile.hpp"

extern Shader world_shader;

struct World {
    std::vector<TILE> grid;
    Shader shader;
    usize level_width;

    IndexedVerts verts;

    World() : shader(world_shader) {};

    void init(Level& l);
    void set(ivec2 pos, TILE t);
    TILE get(ivec2 pos);
    void reset_opengl(Level &);
};
