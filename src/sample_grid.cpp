#include "sample_grid.hpp"

#include "cube.hpp"
#include "ivec2.hpp"

float plane_height(float x, float z, vec3 p0, vec3 p1, vec3 p2) {
    vec3 u = { p1[0] - p0[0], p1[1] - p0[1], p1[2] - p0[2] };
    vec3 v = { p2[0] - p0[0], p2[1] - p0[1], p2[2] - p0[2] };
    vec3 n = { u[1] * v[2] - u[2] * v[1], u[2] * v[0] - u[0] * v[2], u[0] * v[1] - u[1] * v[0] };
    return p0[1] - (n[0] * (x - p0[0]) + n[2] * (z - p0[2])) / n[1];
}

vec3 normal_from_heights(float u, float v, float h00, float h10, float h01, float h11) {
    float dhu = (1.0f - v) * (h10 - h00) + v * (h11 - h01);
    float dhv = (1.0f - u) * (h01 - h00) + u * (h11 - h10);

    return normalize(vec3{ -dhu, 1.0f, -dhv });
}

float sample_floor_y(std::vector<WorldVertex> &grid, float world_x, float world_z, int wsize,
                     vec3 &out_normal) {
    int ix = (int)floorf(world_x);
    int iz = (int)floorf(world_z);

    float u = world_x - (float)ix; // 0-1
    float v = world_z - (float)iz; // 0-1

    const u32 offset = N_VERTS_PER_FACE * N_FACES;
    size idx = ivec2_to_idx((ivec2){ ix, iz }, wsize) * offset;
    float h00 = grid[idx + GROUND_FACE_IDX + 0].pos[1]; // TL
    float h10 = grid[idx + GROUND_FACE_IDX + 1].pos[1]; // TR
    float h01 = grid[idx + GROUND_FACE_IDX + 3].pos[1]; // BR
    float h11 = grid[idx + GROUND_FACE_IDX + 2].pos[1]; // BL

    vec3 p00 = { (float)ix, h00, (float)iz };
    vec3 p10 = { (float)ix + 1, h10, (float)iz };
    vec3 p01 = { (float)ix, h01, (float)iz + 1 };
    vec3 p11 = { (float)ix + 1, h11, (float)iz + 1 };

    out_normal = normal_from_heights(u, v, h00, h10, h01, h11);

    float y;
    if (u + v <= 1.0f) {
        y = plane_height(world_x, world_z, p00, p10, p01);
    } else {
        y = plane_height(world_x, world_z, p11, p01, p10);
    }

    return y;
}
