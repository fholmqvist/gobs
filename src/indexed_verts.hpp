#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "vertices.hpp"

struct IndexedVerts {
    std::vector<WorldVertex> verts;
    std::vector<u32> indices;
};
