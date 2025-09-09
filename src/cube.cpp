#include "cube.hpp"

#include "base.hpp"
#include "constants.hpp"
#include "indexed_verts.hpp"
#include "ivec2.hpp"
#include "tile.hpp"
#include "world.hpp"

const u8 INDICES[N_INDICES] = {
    // +X face (verts 0..3)
    0, 1, 2, 2, 3, 0,
    // -X face (4..7)
    4, 5, 6, 6, 7, 4,
    // +Y face (8..11)
    8, 9, 10, 10, 11, 8,
    // -Y face (12..15)
    12, 13, 14, 14, 15, 12,
    // +Z face (16..19)
    16, 17, 18, 18, 19, 16,
    // -Z face (20..23)
    20, 21, 22, 22, 23, 20
};

static inline i8 face_index_from_dir(FACE_DIR dir) {
    switch (dir) {
        case FACE_DIR::FRONT:
            return 0;
        case FACE_DIR::BACK:
            return 1;
        case FACE_DIR::LEFT:
            return 2;
        case FACE_DIR::RIGHT:
            return 3;
        case FACE_DIR::TOP:
            return 4;
        case FACE_DIR::GROUND:
            return 5;
        default:
            return -1;
    }
}

void faces_for_uv(WorldVertex* vs, u32 idx, TileUV uv, int wsize);

Cube::Cube(TileUV uv, u32 _idx, int wsize) {
    reset(uv, _idx, wsize);
}

void Cube::reset(TileUV uv, u32 _idx, int wsize) {
    idx = _idx;

    WorldVertex vs[24];
    faces_for_uv(vs, idx, uv, wsize);

    assert(sizeof(verts) == sizeof(vs));
    std::copy(std::begin(vs), std::end(vs), verts.begin());

    indices.fill(0);
}

void Cube::on(FACE_DIR dir) {
    int f = face_index_from_dir(dir);
    if (f < 0) {
        return;
    }

    u16 idxBase = (u16)(f * N_INDICES_PER_FACE);
    const u8* src = &INDICES[idxBase];

    for (int i = 0; i < N_INDICES_PER_FACE; ++i) {
        indices[idxBase + i] = src[i];
    }
}

void Cube::off(FACE_DIR dir) {
    i8 f = face_index_from_dir(dir);
    if (f < 0) {
        return;
    }

    u8 base = (u8)(f * N_INDICES_PER_FACE);
    u8 first = (u8)(f * N_VERTS_PER_FACE);

    for (size i = 0; i < N_INDICES_PER_FACE; ++i) {
        indices[base + i] = first;
    }
}

void Cube::set_uv(FACE_DIR dir, TileUV uv, int wsize) {
    i8 f = face_index_from_dir(dir);
    if (f < 0) {
        return;
    }

    // TODO: One for each face dir, not the whole thing.
    WorldVertex vs[24] = {};
    faces_for_uv(vs, idx, uv, wsize);

    u8 face_offset = (u8)(f * N_VERTS_PER_FACE);

    verts[face_offset + 0].uv = vs[face_offset + 0].uv;
    verts[face_offset + 1].uv = vs[face_offset + 1].uv;
    verts[face_offset + 2].uv = vs[face_offset + 2].uv;
    verts[face_offset + 3].uv = vs[face_offset + 3].uv;
}

void Cube::offset(float x, float y, float z) {
    for (size i = 0; i < N_VERTS_PER_CUBE; i++) {
        verts[i].pos[0] += x;
        verts[i].pos[1] += y;
        verts[i].pos[2] += z;
    }
}

void Cube::add_verts_and_indices(IndexedVerts &grid) {
    u32 base_vertex = (u32)grid.verts.size();

    grid.verts.insert(grid.verts.end(), verts.begin(), verts.end());

    grid.indices.reserve(grid.indices.size() + N_INDICES);
    for (u32 n = 0; n < N_INDICES; ++n) {
        grid.indices.push_back((u32)indices[n] + base_vertex);
    }
}

void Cube::update_verts_and_indices(IndexedVerts &grid, size idx) {
    u32 base_vertex = idx * N_VERTS_PER_CUBE;
    u32 base_index = idx * N_INDICES;

    std::copy(verts.begin(), verts.end(), grid.verts.begin() + base_vertex);

    for (u32 n = 0; n < N_INDICES; ++n) {
        grid.indices[base_index + n] = indices[n] + base_vertex;
    }
}

void Cube::is_water(Neighbors walls, Neighbors water, float amount) {
    // TL
    if (!walls.up_left() && !water.up_left() && water.right() && water.down()) {
        verts[20].pos[0] += amount * 0.7f;
        verts[20].pos[2] += amount * 0.7f;
        verts[20].color = SHADOW_STRENGTH;
        verts[21].color = SHADOW_STRENGTH;
        verts[23].color = SHADOW_STRENGTH;
    }
    // TR
    if (!walls.up_right() && !water.up_right() && water.left() && water.down()) {
        verts[21].pos[0] -= amount * 0.7f;
        verts[21].pos[2] += amount * 0.7f;
        verts[20].color = SHADOW_STRENGTH;
        verts[21].color = SHADOW_STRENGTH;
        verts[22].color = SHADOW_STRENGTH;
    }
    // BR
    if (!walls.down_right() && !water.down_right() && water.left() && water.up()) {
        verts[22].pos[0] -= amount * 0.7f;
        verts[22].pos[2] -= amount * 0.7f;
        verts[21].color = SHADOW_STRENGTH;
        verts[22].color = SHADOW_STRENGTH;
        verts[23].color = SHADOW_STRENGTH;
    }
    // BL
    if (!walls.down_left() && !water.down_left() && water.right() && water.up()) {
        verts[23].pos[0] += amount * 0.7f;
        verts[23].pos[2] -= amount * 0.7f;
        verts[20].color = SHADOW_STRENGTH;
        verts[22].color = SHADOW_STRENGTH;
        verts[23].color = SHADOW_STRENGTH;
    }
    // T
    if (!walls.up() && !water.up() && water.left() && water.right()) {
        verts[20].pos[0] -= amount * 0.7f;
        verts[20].pos[2] -= amount * 0.7f;
        verts[21].pos[0] += amount * 0.7f;
        verts[21].pos[2] -= amount * 0.7f;
        verts[22].color = 1.0f;
        verts[23].color = 1.0f;
    }
    // R
    if (!walls.right() && !water.right() && water.up() && water.down()) {
        verts[21].pos[0] += amount * 0.7f;
        verts[21].pos[2] -= amount * 0.7f;
        verts[22].pos[0] += amount * 0.7f;
        verts[22].pos[2] += amount * 0.7f;
        verts[20].color = 1.0f;
        verts[23].color = 1.0f;
    }
    // D
    if (!walls.down() && !water.down() && water.left() && water.right()) {
        verts[22].pos[0] += amount * 0.7f;
        verts[22].pos[2] += amount * 0.7f;
        verts[23].pos[0] -= amount * 0.7f;
        verts[23].pos[2] += amount * 0.7f;
        verts[20].color = 1.0f;
        verts[21].color = 1.0f;
    }
    // L
    if (!walls.left() && !water.left() && water.up() && water.down()) {
        verts[20].pos[0] -= amount * 0.7f;
        verts[20].pos[2] -= amount * 0.7f;
        verts[23].pos[0] -= amount * 0.7f;
        verts[23].pos[2] += amount * 0.7f;
        verts[21].color = 1.0f;
        verts[22].color = 1.0f;
    }
}

void Cube::is_adjacent_to_water(Neighbors walls, Neighbors water, float amount) {
    // TL CORNER, CORNER QUAD
    if (water.down_right() && !water.right() && !water.down()) {
        verts[22].pos[0] += amount * 0.7f;
        verts[22].pos[2] += amount * 0.7f;
        verts[22].color = SHADOW_STRENGTH;
    }
    // TL CORNER, QUAD ABOVE
    if (water.down() && !water.down_left() && !water.left()) {
        verts[23].pos[0] += amount * 0.7f;
        verts[23].pos[2] += amount * 0.7f;
        verts[22].color = SHADOW_STRENGTH;
        verts[23].color = SHADOW_STRENGTH;
    }
    // TOP
    if (water.down() && !water.up()) {
        verts[22].color = SHADOW_STRENGTH;
        verts[23].color = SHADOW_STRENGTH;
    }
    // TR CORNER, QUAD ABOVE
    if (water.down() && !water.down_right() && !water.right()) {
        if (!walls.right()) {
            verts[22].pos[0] -= amount * 0.7f;
        }
        verts[22].pos[2] += amount * 0.7f;
        verts[22].color = SHADOW_STRENGTH;
        verts[23].color = SHADOW_STRENGTH;
    }
    // TR CORNER, CORNER QUAD
    if (water.down_left() && !water.left() && !water.down()) {
        verts[23].pos[0] -= amount * 0.7f;
        verts[23].pos[2] += amount * 0.7f;
        verts[23].color = SHADOW_STRENGTH;
    }
    // TR CORNER, RIGHT QUAD
    if (water.left() && !water.up_left() && !water.down()) {
        verts[20].pos[0] -= amount * 0.7f;
        verts[20].pos[2] += amount * 0.7f;
        verts[20].color = SHADOW_STRENGTH;
        verts[23].color = SHADOW_STRENGTH;
    }
    // RIGHT
    if (water.left() && !water.right()) {
        verts[20].color = SHADOW_STRENGTH;
        verts[23].color = SHADOW_STRENGTH;
    }
    // BR CORNER, RIGHT QUAD
    if (water.left() && !water.down_left() && !water.up()) {
        verts[23].pos[0] -= amount * 0.7f;
        verts[23].pos[2] -= amount * 0.7f;
        verts[20].color = SHADOW_STRENGTH;
        verts[23].color = SHADOW_STRENGTH;
    }
    // BR CORNER, CORNER QUAD
    if (water.up_left() && !water.left() && !water.up()) {
        verts[20].pos[0] -= amount * 0.7f;
        verts[20].pos[2] -= amount * 0.7f;
        verts[20].color = SHADOW_STRENGTH;
    }
    // BR CORNER, QUAD BELOW
    if (water.up() && !water.up_right() && !water.right()) {
        verts[21].pos[0] -= amount * 0.7f;
        verts[21].pos[2] -= amount * 0.7f;
        verts[20].color = SHADOW_STRENGTH;
        verts[21].color = SHADOW_STRENGTH;
    }
    // DOWN
    if (water.up() && !water.down()) {
        verts[20].color = SHADOW_STRENGTH;
        verts[21].color = SHADOW_STRENGTH;
    }
    // BL CORNER, QUAD BELOW
    if (water.up() && !water.up_left() && !water.left()) {
        verts[20].pos[0] += amount * 0.7f;
        verts[20].pos[2] -= amount * 0.7f;
        verts[20].color = SHADOW_STRENGTH;
        verts[21].color = SHADOW_STRENGTH;
    }
    // BL CORNER, CORNER QUAD
    if (water.up_right() && !water.up() && !water.right()) {
        verts[21].pos[0] += amount * 0.7f;
        verts[21].pos[2] -= amount * 0.7f;
        verts[21].color = SHADOW_STRENGTH;
    }
    // BL CORNER, QUAD LEFT
    if (water.right() && !water.down_right() && !water.down()) {
        verts[22].pos[0] += amount * 0.7f;
        if (!walls.down()) {
            verts[22].pos[2] -= amount * 0.7f;
        }
        verts[21].color = SHADOW_STRENGTH;
        verts[22].color = SHADOW_STRENGTH;
    }
    // LEFT
    if (water.right() && !water.left()) {
        verts[21].color = SHADOW_STRENGTH;
        verts[22].color = SHADOW_STRENGTH;
    }
    // TL CORNER, QUAD LEFT
    if (water.right() && !water.up_right() && !water.up()) {
        verts[21].pos[0] += amount * 0.7f;
        verts[21].pos[2] += amount * 0.7f;
        verts[21].color = SHADOW_STRENGTH;
        verts[22].color = SHADOW_STRENGTH;
    }
}

void Cube::shade_unshaded_corners(Neighbors walls) {
    const float BRIGHT_SHADOW = 0.65f;
    if (walls.up()) {
        verts[20].color = min(BRIGHT_SHADOW, verts[20].color);
        verts[21].color = min(BRIGHT_SHADOW, verts[21].color);
    }
    if (walls.up_right()) {
        verts[21].color = min(BRIGHT_SHADOW, verts[21].color);
    }
    if (walls.right()) {
        verts[21].color = min(BRIGHT_SHADOW, verts[21].color);
        verts[22].color = min(BRIGHT_SHADOW, verts[22].color);
    }
    if (walls.down_right()) {
        verts[22].color = min(BRIGHT_SHADOW, verts[22].color);
    }
    if (walls.down()) {
        verts[22].color = min(BRIGHT_SHADOW, verts[22].color);
        verts[23].color = min(BRIGHT_SHADOW, verts[23].color);
    }
    if (walls.down_left()) {
        verts[23].color = min(BRIGHT_SHADOW, verts[23].color);
    }
    if (walls.left()) {
        verts[20].color = min(BRIGHT_SHADOW, verts[20].color);
        verts[23].color = min(BRIGHT_SHADOW, verts[23].color);
    }
    if (walls.up_left()) {
        verts[20].color = min(BRIGHT_SHADOW, verts[20].color);
    }
}

void faces_for_uv(WorldVertex* vs, u32 idx, TileUV uv, int wsize) {
    float COL = (float)(u32)(idx % wsize);
    float ROW = (float)(u32)(idx / wsize);

    // FRONT
    vs[0] = { { -0.5f, -0.5f, 0.5f }, { 0, 0, 1 }, { uv.x1, uv.y1 }, { COL, ROW }, 1 };
    vs[1] = { { -0.5f, 0.5f, 0.5f }, { 0, 0, 1 }, { uv.x1, uv.y2 }, { COL, ROW }, 0 };
    vs[2] = { { 0.5f, 0.5f, 0.5f }, { 0, 0, 1 }, { uv.x2, uv.y2 }, { COL, ROW }, 0 };
    vs[3] = { { 0.5f, -0.5f, 0.5f }, { 0, 0, 1 }, { uv.x2, uv.y1 }, { COL, ROW }, 1 };
    // BACK
    vs[4] = { { 0.5f, -0.5f, -0.5f }, { 0, 0, -1 }, { uv.x1, uv.y1 }, { COL, ROW }, 1 };
    vs[5] = { { 0.5f, 0.5f, -0.5f }, { 0, 0, -1 }, { uv.x1, uv.y2 }, { COL, ROW }, 0 };
    vs[6] = { { -0.5f, 0.5f, -0.5f }, { 0, 0, -1 }, { uv.x2, uv.y2 }, { COL, ROW }, 0 };
    vs[7] = { { -0.5f, -0.5f, -0.5f }, { 0, 0, -1 }, { uv.x2, uv.y1 }, { COL, ROW }, 1 };
    // LEFT
    vs[8] = { { -0.5f, -0.5f, -0.5f }, { -1, 0, 0 }, { uv.x1, uv.y1 }, { COL, ROW }, 1 };
    vs[9] = { { -0.5f, 0.5f, -0.5f }, { -1, 0, 0 }, { uv.x1, uv.y2 }, { COL, ROW }, 0 };
    vs[10] = { { -0.5f, 0.5f, 0.5f }, { -1, 0, 0 }, { uv.x2, uv.y2 }, { COL, ROW }, 0 };
    vs[11] = { { -0.5f, -0.5f, 0.5f }, { -1, 0, 0 }, { uv.x2, uv.y1 }, { COL, ROW }, 1 };
    // RIGHT
    vs[12] = { { 0.5f, -0.5f, 0.5f }, { 1, 0, 0 }, { uv.x1, uv.y1 }, { COL, ROW }, 1 };
    vs[13] = { { 0.5f, 0.5f, 0.5f }, { 1, 0, 0 }, { uv.x1, uv.y2 }, { COL, ROW }, 0 };
    vs[14] = { { 0.5f, 0.5f, -0.5f }, { 1, 0, 0 }, { uv.x2, uv.y2 }, { COL, ROW }, 0 };
    vs[15] = { { 0.5f, -0.5f, -0.5f }, { 1, 0, 0 }, { uv.x2, uv.y1 }, { COL, ROW }, 1 };
    // TOP
    vs[16] = { { -0.5f, 0.5f, -0.5f }, { 0, 1, 0 }, { uv.x1, uv.y1 }, { COL, ROW }, 0 };
    vs[17] = { { 0.5f, 0.5f, -0.5f }, { 0, 1, 0 }, { uv.x2, uv.y1 }, { COL, ROW }, 0 };
    vs[18] = { { 0.5f, 0.5f, 0.5f }, { 0, 1, 0 }, { uv.x2, uv.y2 }, { COL, ROW }, 0 };
    vs[19] = { { -0.5f, 0.5f, 0.5f }, { 0, 1, 0 }, { uv.x1, uv.y2 }, { COL, ROW }, 0 };
    // GROUND
    vs[20] = { { -0.5f, -0.5f, -0.5f }, { 0, 1, 0 }, { uv.x1, uv.y1 }, { COL, ROW }, 1 };
    vs[21] = { { 0.5f, -0.5f, -0.5f }, { 0, 1, 0 }, { uv.x2, uv.y1 }, { COL, ROW }, 1 };
    vs[22] = { { 0.5f, -0.5f, 0.5f }, { 0, 1, 0 }, { uv.x2, uv.y2 }, { COL, ROW }, 1 };
    vs[23] = { { -0.5f, -0.5f, 0.5f }, { 0, 1, 0 }, { uv.x1, uv.y2 }, { COL, ROW }, 1 };
}

void Cube::fix_nbs_wall_uvs(World &world, Neighbors walls, int wsize) {
    ivec2 vdx;

    if (!walls.up()) {
        vdx = ivec2_from_idx(idx, wsize);
        vdx[1]--;
        if (in_range(vdx, wsize)) {
            TILE t = world.get(vdx);
            set_uv(FACE_DIR::BACK, tile_get_matching_uvs(t), wsize);
        }
    }
    if (!walls.right()) {
        vdx = ivec2_from_idx(idx, wsize);
        vdx[0]++;
        if (in_range(vdx, wsize)) {
            TILE t = world.get(vdx);
            set_uv(FACE_DIR::RIGHT, tile_get_matching_uvs(t), wsize);
        }
    }
    if (!walls.down()) {
        vdx = ivec2_from_idx(idx, wsize);
        vdx[1]++;
        if (in_range(vdx, wsize)) {
            TILE t = world.get(vdx);
            set_uv(FACE_DIR::FRONT, tile_get_matching_uvs(t), wsize);
        }
    }
    if (!walls.left()) {
        vdx = ivec2_from_idx(idx, wsize);
        vdx[0]--;
        if (in_range(vdx, wsize)) {
            TILE t = world.get(vdx);
            set_uv(FACE_DIR::LEFT, tile_get_matching_uvs(t), wsize);
        }
    }
}
