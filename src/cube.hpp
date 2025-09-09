#pragma once

#include "indexed_verts.hpp"
#include "neighbors.hpp"
#include "tile_uv.hpp"
#include "vertices.hpp"
#include "world.hpp"

// clang-format off
#define N_VERTS_PER_FACE   4
#define N_FACES            6
#define N_VERTS_PER_CUBE   (N_FACES * N_VERTS_PER_FACE)
#define N_INDICES_PER_FACE 6
#define N_INDICES          (N_FACES * N_INDICES_PER_FACE)
#define VERT_STRIDE        (size)sizeof(WorldVertex)
#define CUBE_VERTICES      (usize)(VERT_STRIDE * N_VERTS_PER_FACE * FACE_DIR_COUNT)
#define CUBE_INDICES       (usize)(N_INDICES_PER_FACE * (usize)FACE_DIR::COUNT)
#define FLOATS_PER_VERTEX  (u32)(sizeof(WorldVertex) / sizeof(float))

#define FRONT_FACE_IDX     0
#define BACK_FACE_IDX      4
#define LEFT_FACE_IDX      8
#define RIGHT_FACE_IDX     12
#define TOP_FACE_IDX       16
#define GROUND_FACE_IDX    20
// clang-format on

enum struct FACE_DIR : u8 {
    NONE,
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    TOP,
    GROUND,
    COUNT
};

struct Cube {
    std::array<WorldVertex, N_VERTS_PER_CUBE> verts;
    std::array<u8, N_INDICES> indices;
    u32 idx;

    Cube() = default;
    Cube(TileUV uv, u32 idx, int wsize);

    void reset(TileUV uv, u32 idx, int wsize);
    void on(FACE_DIR dir);
    void off(FACE_DIR dir);
    void set_uv(FACE_DIR dir, TileUV uv, int wsize);
    void offset(float x, float y, float z);
    void add_verts_and_indices(IndexedVerts &grid);
    void update_verts_and_indices(IndexedVerts &grid, size idx);
    void fix_nbs_wall_uvs(World &world, std::bitset<8> walls, int wsize);
    void is_water(Neighbors walls, Neighbors water, float amount);
    void is_adjacent_to_water(Neighbors walls, Neighbors water, float amount);
    void shade_unshaded_corners(Neighbors walls);
    void fix_nbs_wall_uvs(World &world, Neighbors walls, int wsize);
};
