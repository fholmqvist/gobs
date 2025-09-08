#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "indexed_verts.hpp"
#include "shader.hpp"

extern Shader world_shader;

struct World {
    std::vector<u8> grid;
    Shader shader;

    IndexedVerts verts;

    World() : shader(world_shader) {};
};
