#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "base.hpp"

ivec2 ivec2_from_idx(size idx, usize wsize);
size ivec2_to_idx(ivec2 v, usize wsize);
void ivec2_add_csize(ivec2 v);
