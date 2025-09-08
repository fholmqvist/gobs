#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "indexed_verts.hpp"
#include "shader.hpp"
#include "tile.hpp"

extern Shader world_shader;

struct World {
    std::vector<Tile> grid;
    Shader shader;
    usize level_width;

    IndexedVerts verts;

    World() : shader(world_shader) {};

    Tile get(ivec2 pos);
    void reset_opengl(Level &);
};
