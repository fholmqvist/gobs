#include "liquid.hpp"

#include "constants.hpp"
#include "cube.hpp"
#include "ivec2.hpp"
#include "lattice.hpp"
#include "random.hpp"
#include "tile_uv.hpp"

ID LIQUID_GLOBAL_ID = 0;

Liquid::Liquid(LIQUID t, ivec4 wpos, int wsize) {
    id = ++LIQUID_GLOBAL_ID;
    type = t;

    TileUV liquid_uv = (TileUV){ 0, 1, 0, 1 };

    Cube c(liquid_uv, 0, wsize);

    Lattice l;

    usize count = (usize)(abs(wpos[2] - wpos[0]) * abs(wpos[3] - wpos[1]));

    verts.verts.reserve(count * N_VERTS_PER_CUBE);
    verts.indices.reserve(count * CUBE_INDICES);

    const float y_offset = 0.0f;

    for (size y = wpos[1]; y < wpos[3]; y++) {
        for (size x = wpos[0]; x < wpos[2]; x++) {
            u32 idx = (u32)ivec2_to_idx({ (int)x, (int)y }, wsize);

            c.reset(liquid_uv, idx, wsize);
            c.on(FACE_DIR::GROUND);

            l.from_world_vertices(c.verts, true);
            l.add_slope_perlin(vec3{ (float)x, 0, (float)y }, y_offset, PERLIN_DEFAULT_STRENGTH);
            l.add_offset(WATER_LEVEL * 1.22f);
            l.apply_to_cube(c);

            if (t == LIQUID::OIL) {
                for (size i = 0; i < 24; i++) {
                    c.verts[i].color = 0.072f;
                }
            }

            c.offset(0.5f, 0, 0.5f);
            c.add_verts_and_indices(verts);
        }
    }

    // IMPORTANT!
    //
    // Note that we're storing the original
    // Y position in an unused float position.
    original_ys.reserve(count * N_VERTS_PER_CUBE);
    for (size i = 0; i < (size)verts.verts.size(); i++) {
        original_ys.push_back(verts.verts[i].pos[1]);
    }
}

static float WATER_CYCLE = 0.0f;

void Liquid::update() {
    for (size i = 0; i < (size)verts.verts.size(); i++) {
        verts.verts[i].pos[1] = original_ys[i];

        ivec2 pos = {
            (int)verts.verts[i].pos[0] + (int)verts.verts[i].wpos[0],
            (int)verts.verts[i].pos[2] + (int)verts.verts[i].wpos[1],
        };

        float speed = type == LIQUID::OIL ? 0.4f : 1.0f;
        float amount = (float)(pos[0] + pos[1]) + WATER_CYCLE * speed;

        amount = sinf(amount) + 1.0f;
        verts.verts[i].pos[1] += amount / 128.0f;
    }

    WATER_CYCLE += DELTA_TIME * 1.5f;
}
