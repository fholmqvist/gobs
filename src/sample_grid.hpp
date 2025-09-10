#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "vertices.hpp"

float sample_floor_y(std::vector<WorldVertex> &grid, float world_x, float world_z, int wsize, vec3& out_normal);
