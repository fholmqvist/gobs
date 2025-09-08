#pragma once

// clang-format off
#define N_VERTS_PER_FACE   4
#define N_FACES            6
#define N_VERTS_PER_CUBE   (N_FACES * N_VERTS_PER_FACE)
#define N_INDICES_PER_FACE 6
#define N_INDICES          (N_FACES * N_INDICES_PER_FACE)
#define VERT_STRIDE        (size)sizeof(WorldVertex)
#define CUBE_VERTICES      (usize)(VERT_STRIDE * N_VERTS_PER_FACE * FACE_DIR_COUNT)
#define CUBE_INDICES       (usize)(N_INDICES_PER_FACE * FACE_DIR_COUNT)
#define FLOATS_PER_VERTEX  (u32)(sizeof(WorldVertex) / sizeof(float))

#define FRONT_FACE_IDX     0
#define BACK_FACE_IDX      4
#define LEFT_FACE_IDX      8
#define RIGHT_FACE_IDX     12
#define TOP_FACE_IDX       16
#define GROUND_FACE_IDX    20
// clang-format on
