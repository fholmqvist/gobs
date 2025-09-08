#include "cube.hpp"

#include "base.hpp"
#include "constants.hpp"
#include "indexed_verts.hpp"
#include "ivec2.hpp"
#include "nbs.hpp"
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

void Cube::init(TileUV uv, u32 _idx, int wsize) {
    idx = _idx;

    WorldVertex vs[24];
    faces_for_uv(vs, idx, uv, wsize);

    assert(sizeof(verts) == sizeof(vs));
    std::copy(std::begin(vs), std::end(vs), verts.begin());

    indices.fill(0);
}

void Cube::reset(TileUV uv, u32 idx, int wsize) {
    init(uv, idx, wsize);
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

    grid.verts.reserve(grid.verts.size() + N_VERTS_PER_CUBE);
    for (u32 n = 0; n < N_VERTS_PER_CUBE; ++n) {
        grid.verts.push_back(verts[n]);
    }

    grid.indices.reserve(grid.indices.size() + N_INDICES);
    for (u32 n = 0; n < N_INDICES; ++n) {
        grid.indices.push_back((u32)(indices[n] + base_vertex));
    }
}

void cube_is_water(Cube* c, u8 walls, u8 water, float amount) {
    // TL
    if (!nbs_up_left(walls) && !nbs_up_left(water) && nbs_right(water) && nbs_down(water)) {
        c->verts[20].pos[0] += amount * 0.7f;
        c->verts[20].pos[2] += amount * 0.7f;
        c->verts[20].color = SHADOW_STRENGTH;
        c->verts[21].color = SHADOW_STRENGTH;
        c->verts[23].color = SHADOW_STRENGTH;
    }
    // TR
    if (!nbs_up_right(walls) && !nbs_up_right(water) && nbs_left(water) && nbs_down(water)) {
        c->verts[21].pos[0] -= amount * 0.7f;
        c->verts[21].pos[2] += amount * 0.7f;
        c->verts[20].color = SHADOW_STRENGTH;
        c->verts[21].color = SHADOW_STRENGTH;
        c->verts[22].color = SHADOW_STRENGTH;
    }
    // BR
    if (!nbs_down_right(walls) && !nbs_down_right(water) && nbs_left(water) && nbs_up(water)) {
        c->verts[22].pos[0] -= amount * 0.7f;
        c->verts[22].pos[2] -= amount * 0.7f;
        c->verts[21].color = SHADOW_STRENGTH;
        c->verts[22].color = SHADOW_STRENGTH;
        c->verts[23].color = SHADOW_STRENGTH;
    }
    // BL
    if (!nbs_down_left(walls) && !nbs_down_left(water) && nbs_right(water) && nbs_up(water)) {
        c->verts[23].pos[0] += amount * 0.7f;
        c->verts[23].pos[2] -= amount * 0.7f;
        c->verts[20].color = SHADOW_STRENGTH;
        c->verts[22].color = SHADOW_STRENGTH;
        c->verts[23].color = SHADOW_STRENGTH;
    }
    // T
    if (!nbs_up(walls) && !nbs_up(water) && nbs_left(water) && nbs_right(water)) {
        c->verts[20].pos[0] -= amount * 0.7f;
        c->verts[20].pos[2] -= amount * 0.7f;
        c->verts[21].pos[0] += amount * 0.7f;
        c->verts[21].pos[2] -= amount * 0.7f;
        c->verts[22].color = 1.0f;
        c->verts[23].color = 1.0f;
    }
    // R
    if (!nbs_right(walls) && !nbs_right(water) && nbs_up(water) && nbs_down(water)) {
        c->verts[21].pos[0] += amount * 0.7f;
        c->verts[21].pos[2] -= amount * 0.7f;
        c->verts[22].pos[0] += amount * 0.7f;
        c->verts[22].pos[2] += amount * 0.7f;
        c->verts[20].color = 1.0f;
        c->verts[23].color = 1.0f;
    }
    // D
    if (!nbs_down(walls) && !nbs_down(water) && nbs_left(water) && nbs_right(water)) {
        c->verts[22].pos[0] += amount * 0.7f;
        c->verts[22].pos[2] += amount * 0.7f;
        c->verts[23].pos[0] -= amount * 0.7f;
        c->verts[23].pos[2] += amount * 0.7f;
        c->verts[20].color = 1.0f;
        c->verts[21].color = 1.0f;
    }
    // L
    if (!nbs_left(walls) && !nbs_left(water) && nbs_up(water) && nbs_down(water)) {
        c->verts[20].pos[0] -= amount * 0.7f;
        c->verts[20].pos[2] -= amount * 0.7f;
        c->verts[23].pos[0] -= amount * 0.7f;
        c->verts[23].pos[2] += amount * 0.7f;
        c->verts[21].color = 1.0f;
        c->verts[22].color = 1.0f;
    }
}

void cube_is_adjacent_to_water(Cube* c, u8 walls, u8 water, float amount) {
    // TL CORNER, CORNER QUAD
    if (nbs_down_right(water) && !nbs_right(water) && !nbs_down(water)) {
        c->verts[22].pos[0] += amount * 0.7f;
        c->verts[22].pos[2] += amount * 0.7f;
        c->verts[22].color = SHADOW_STRENGTH;
    }
    // TL CORNER, QUAD ABOVE
    if (nbs_down(water) && !nbs_down_left(water) && !nbs_left(water)) {
        c->verts[23].pos[0] += amount * 0.7f;
        c->verts[23].pos[2] += amount * 0.7f;
        c->verts[22].color = SHADOW_STRENGTH;
        c->verts[23].color = SHADOW_STRENGTH;
    }
    // TOP
    if (nbs_down(water) && !nbs_up(water)) {
        c->verts[22].color = SHADOW_STRENGTH;
        c->verts[23].color = SHADOW_STRENGTH;
    }
    // TR CORNER, QUAD ABOVE
    if (nbs_down(water) && !nbs_down_right(water) && !nbs_right(water)) {
        if (!nbs_right(walls)) {
            c->verts[22].pos[0] -= amount * 0.7f;
        }
        c->verts[22].pos[2] += amount * 0.7f;
        c->verts[22].color = SHADOW_STRENGTH;
        c->verts[23].color = SHADOW_STRENGTH;
    }
    // TR CORNER, CORNER QUAD
    if (nbs_down_left(water) && !nbs_left(water) && !nbs_down(water)) {
        c->verts[23].pos[0] -= amount * 0.7f;
        c->verts[23].pos[2] += amount * 0.7f;
        c->verts[23].color = SHADOW_STRENGTH;
    }
    // TR CORNER, RIGHT QUAD
    if (nbs_left(water) && !nbs_up_left(water) && !nbs_down(water)) {
        c->verts[20].pos[0] -= amount * 0.7f;
        c->verts[20].pos[2] += amount * 0.7f;
        c->verts[20].color = SHADOW_STRENGTH;
        c->verts[23].color = SHADOW_STRENGTH;
    }
    // RIGHT
    if (nbs_left(water) && !nbs_right(water)) {
        c->verts[20].color = SHADOW_STRENGTH;
        c->verts[23].color = SHADOW_STRENGTH;
    }
    // BR CORNER, RIGHT QUAD
    if (nbs_left(water) && !nbs_down_left(water) && !nbs_up(water)) {
        c->verts[23].pos[0] -= amount * 0.7f;
        c->verts[23].pos[2] -= amount * 0.7f;
        c->verts[20].color = SHADOW_STRENGTH;
        c->verts[23].color = SHADOW_STRENGTH;
    }
    // BR CORNER, CORNER QUAD
    if (nbs_up_left(water) && !nbs_left(water) && !nbs_up(water)) {
        c->verts[20].pos[0] -= amount * 0.7f;
        c->verts[20].pos[2] -= amount * 0.7f;
        c->verts[20].color = SHADOW_STRENGTH;
    }
    // BR CORNER, QUAD BELOW
    if (nbs_up(water) && !nbs_up_right(water) && !nbs_right(water)) {
        c->verts[21].pos[0] -= amount * 0.7f;
        c->verts[21].pos[2] -= amount * 0.7f;
        c->verts[20].color = SHADOW_STRENGTH;
        c->verts[21].color = SHADOW_STRENGTH;
    }
    // DOWN
    if (nbs_up(water) && !nbs_down(water)) {
        c->verts[20].color = SHADOW_STRENGTH;
        c->verts[21].color = SHADOW_STRENGTH;
    }
    // BL CORNER, QUAD BELOW
    if (nbs_up(water) && !nbs_up_left(water) && !nbs_left(water)) {
        c->verts[20].pos[0] += amount * 0.7f;
        c->verts[20].pos[2] -= amount * 0.7f;
        c->verts[20].color = SHADOW_STRENGTH;
        c->verts[21].color = SHADOW_STRENGTH;
    }
    // BL CORNER, CORNER QUAD
    if (nbs_up_right(water) && !nbs_up(water) && !nbs_right(water)) {
        c->verts[21].pos[0] += amount * 0.7f;
        c->verts[21].pos[2] -= amount * 0.7f;
        c->verts[21].color = SHADOW_STRENGTH;
    }
    // BL CORNER, QUAD LEFT
    if (nbs_right(water) && !nbs_down_right(water) && !nbs_down(water)) {
        c->verts[22].pos[0] += amount * 0.7f;
        if (!nbs_down(walls)) {
            c->verts[22].pos[2] -= amount * 0.7f;
        }
        c->verts[21].color = SHADOW_STRENGTH;
        c->verts[22].color = SHADOW_STRENGTH;
    }
    // LEFT
    if (nbs_right(water) && !nbs_left(water)) {
        c->verts[21].color = SHADOW_STRENGTH;
        c->verts[22].color = SHADOW_STRENGTH;
    }
    // TL CORNER, QUAD LEFT
    if (nbs_right(water) && !nbs_up_right(water) && !nbs_up(water)) {
        c->verts[21].pos[0] += amount * 0.7f;
        c->verts[21].pos[2] += amount * 0.7f;
        c->verts[21].color = SHADOW_STRENGTH;
        c->verts[22].color = SHADOW_STRENGTH;
    }
}

void cube_shade_unshaded_corners(Cube* c, u8 walls) {
    const float BRIGHT_SHADOW = 0.65f;
    if (nbs_up(walls)) {
        c->verts[20].color = min(BRIGHT_SHADOW, c->verts[20].color);
        c->verts[21].color = min(BRIGHT_SHADOW, c->verts[21].color);
    }
    if (nbs_up_right(walls)) {
        c->verts[21].color = min(BRIGHT_SHADOW, c->verts[21].color);
    }
    if (nbs_right(walls)) {
        c->verts[21].color = min(BRIGHT_SHADOW, c->verts[21].color);
        c->verts[22].color = min(BRIGHT_SHADOW, c->verts[22].color);
    }
    if (nbs_down_right(walls)) {
        c->verts[22].color = min(BRIGHT_SHADOW, c->verts[22].color);
    }
    if (nbs_down(walls)) {
        c->verts[22].color = min(BRIGHT_SHADOW, c->verts[22].color);
        c->verts[23].color = min(BRIGHT_SHADOW, c->verts[23].color);
    }
    if (nbs_down_left(walls)) {
        c->verts[23].color = min(BRIGHT_SHADOW, c->verts[23].color);
    }
    if (nbs_left(walls)) {
        c->verts[20].color = min(BRIGHT_SHADOW, c->verts[20].color);
        c->verts[23].color = min(BRIGHT_SHADOW, c->verts[23].color);
    }
    if (nbs_up_left(walls)) {
        c->verts[20].color = min(BRIGHT_SHADOW, c->verts[20].color);
    }
}

void faces_for_uv(WorldVertex* vs, u32 idx, TileUV uv, int wsize) {
    float COL = (float)(u32)(idx % wsize);
    float ROW = (float)(u32)(idx / wsize);

    // FRONT
    vs[0] = (WorldVertex){ { -0.5f, -0.5f, 0.5f }, { 0, 0, 1 }, { uv.x1, uv.y1 }, { COL, ROW }, 1 };
    vs[1] = (WorldVertex){ { -0.5f, 0.5f, 0.5f }, { 0, 0, 1 }, { uv.x1, uv.y2 }, { COL, ROW }, 0 };
    vs[2] = (WorldVertex){ { 0.5f, 0.5f, 0.5f }, { 0, 0, 1 }, { uv.x2, uv.y2 }, { COL, ROW }, 0 };
    vs[3] = (WorldVertex){ { 0.5f, -0.5f, 0.5f }, { 0, 0, 1 }, { uv.x2, uv.y1 }, { COL, ROW }, 1 };
    // BACK
    vs[4] =
        (WorldVertex){ { 0.5f, -0.5f, -0.5f }, { 0, 0, -1 }, { uv.x1, uv.y1 }, { COL, ROW }, 1 };
    vs[5] = (WorldVertex){ { 0.5f, 0.5f, -0.5f }, { 0, 0, -1 }, { uv.x1, uv.y2 }, { COL, ROW }, 0 };
    vs[6] =
        (WorldVertex){ { -0.5f, 0.5f, -0.5f }, { 0, 0, -1 }, { uv.x2, uv.y2 }, { COL, ROW }, 0 };
    vs[7] =
        (WorldVertex){ { -0.5f, -0.5f, -0.5f }, { 0, 0, -1 }, { uv.x2, uv.y1 }, { COL, ROW }, 1 };
    // LEFT
    vs[8] =
        (WorldVertex){ { -0.5f, -0.5f, -0.5f }, { -1, 0, 0 }, { uv.x1, uv.y1 }, { COL, ROW }, 1 };
    vs[9] =
        (WorldVertex){ { -0.5f, 0.5f, -0.5f }, { -1, 0, 0 }, { uv.x1, uv.y2 }, { COL, ROW }, 0 };
    vs[10] =
        (WorldVertex){ { -0.5f, 0.5f, 0.5f }, { -1, 0, 0 }, { uv.x2, uv.y2 }, { COL, ROW }, 0 };
    vs[11] =
        (WorldVertex){ { -0.5f, -0.5f, 0.5f }, { -1, 0, 0 }, { uv.x2, uv.y1 }, { COL, ROW }, 1 };
    // RIGHT
    vs[12] = (WorldVertex){ { 0.5f, -0.5f, 0.5f }, { 1, 0, 0 }, { uv.x1, uv.y1 }, { COL, ROW }, 1 };
    vs[13] = (WorldVertex){ { 0.5f, 0.5f, 0.5f }, { 1, 0, 0 }, { uv.x1, uv.y2 }, { COL, ROW }, 0 };
    vs[14] = (WorldVertex){ { 0.5f, 0.5f, -0.5f }, { 1, 0, 0 }, { uv.x2, uv.y2 }, { COL, ROW }, 0 };
    vs[15] =
        (WorldVertex){ { 0.5f, -0.5f, -0.5f }, { 1, 0, 0 }, { uv.x2, uv.y1 }, { COL, ROW }, 1 };
    // TOP
    vs[16] =
        (WorldVertex){ { -0.5f, 0.5f, -0.5f }, { 0, 1, 0 }, { uv.x1, uv.y1 }, { COL, ROW }, 0 };
    vs[17] = (WorldVertex){ { 0.5f, 0.5f, -0.5f }, { 0, 1, 0 }, { uv.x2, uv.y1 }, { COL, ROW }, 0 };
    vs[18] = (WorldVertex){ { 0.5f, 0.5f, 0.5f }, { 0, 1, 0 }, { uv.x2, uv.y2 }, { COL, ROW }, 0 };
    vs[19] = (WorldVertex){ { -0.5f, 0.5f, 0.5f }, { 0, 1, 0 }, { uv.x1, uv.y2 }, { COL, ROW }, 0 };
    // GROUND
    vs[20] =
        (WorldVertex){ { -0.5f, -0.5f, -0.5f }, { 0, 1, 0 }, { uv.x1, uv.y1 }, { COL, ROW }, 1 };
    vs[21] =
        (WorldVertex){ { 0.5f, -0.5f, -0.5f }, { 0, 1, 0 }, { uv.x2, uv.y1 }, { COL, ROW }, 1 };
    vs[22] = (WorldVertex){ { 0.5f, -0.5f, 0.5f }, { 0, 1, 0 }, { uv.x2, uv.y2 }, { COL, ROW }, 1 };
    vs[23] =
        (WorldVertex){ { -0.5f, -0.5f, 0.5f }, { 0, 1, 0 }, { uv.x1, uv.y2 }, { COL, ROW }, 1 };
}

void Cube::fix_nbs_wall_uvs(World& world, std::bitset<8> walls, int wsize) {
    ivec2 vdx;

    if (!nbs_up(walls)) {
        vdx = ivec2_from_idx(idx, wsize);
        vdx[1]--;
        if (in_range(vdx, wsize)) {
            TILE t = world.get(vdx);
            set_uv(FACE_DIR::BACK, tile_get_matching_uvs(t), wsize);
        }
    }
    if (!nbs_right(walls)) {
        vdx = ivec2_from_idx(idx, wsize);
        vdx[0]++;
        if (in_range(vdx, wsize)) {
            TILE t = world.get(vdx);
            set_uv(FACE_DIR::RIGHT, tile_get_matching_uvs(t), wsize);
        }
    }
    if (!nbs_down(walls)) {
        vdx = ivec2_from_idx(idx, wsize);
        vdx[1]++;
        if (in_range(vdx, wsize)) {
            TILE t = world.get(vdx);
            set_uv(FACE_DIR::FRONT, tile_get_matching_uvs(t), wsize);
        }
    }
    if (!nbs_left(walls)) {
        vdx = ivec2_from_idx(idx, wsize);
        vdx[0]--;
        if (in_range(vdx, wsize)) {
            TILE t = world.get(vdx);
            set_uv(FACE_DIR::LEFT, tile_get_matching_uvs(t), wsize);
        }
    }
}
