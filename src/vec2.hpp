#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "constants.hpp"

#define I2(x, y) ((int[2]){ x, y })
#define V2(x, y) ((vec2){ x, y })
#define V3(x, y, z) ((vec3){ x, y, z })
#define V3ZERO GLM_VEC3_ZERO
#define F3(x, y, z) ((float[3]){ x, y, z })

static inline void vec2_world_to_grid(vec2 v, ivec2 out) {
    out[0] = TO_GRID(v[0]);
    out[1] = TO_GRID(v[1]);
}

static inline float dist2_to(vec3 p, vec3 center) {
    vec3 d = p - center;
    return d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
}
