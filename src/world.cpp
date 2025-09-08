#include "world.hpp"
#include "cube.hpp"
#include "ivec2.hpp"
#include "lattice.hpp"
#include <glm/fwd.hpp>

Shader world_shader = Shader(
    "assets/shaders/grid_vs.glsl", "assets/shaders/grid_fs.glsl", [](Shader &s, Level &level) {
        (void)level;

        GLsizei stride = VERT_STRIDE;

        glBufferData(GL_UNIFORM_BUFFER, (size)KB(16), NULL, GL_DYNAMIC_DRAW);

        const GLuint BINDING = 0;
        GLuint block = glGetUniformBlockIndex(s.ID, "Lights");
        glUniformBlockBinding(s.ID, block, BINDING);
        glBindBufferBase(GL_UNIFORM_BUFFER, BINDING, s.UBO);

        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

        glEnableVertexAttribArray(1); // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));

        glEnableVertexAttribArray(2); // uv
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

        glEnableVertexAttribArray(3); // wpos
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float)));

        glEnableVertexAttribArray(4); // vcolor
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, stride, (void*)(10 * sizeof(float)));
    });

void World::init(Level &l) {
    level_width = l.width;
    grid.reserve(l.total);
}

void World::set(ivec2 pos, TILE t) {
    grid[ivec2_to_idx(pos, level_width)] = t;
}

TILE World::get(ivec2 pos) {
    return grid[ivec2_to_idx(pos, level_width)];
}

void set_cube(World& world, Cube& c, Lattice& l, u32 i, ivec2 pos, TileUV uv, int wsize);

void World::reset_opengl(Level &l) {
    verts.verts.clear();
    verts.verts.reserve(l.total * N_VERTS_PER_CUBE);
    verts.indices.clear();
    verts.indices.reserve(l.total * CUBE_INDICES);

    Cube cube;
    Lattice lattice;

    for (u32 i = 0; i < l.total; i++) {
        ivec2 pos = ivec2_from_idx(i, l.width);
        TILE tile = get(pos);

        set_cube(*this, cube, lattice, i, pos, tile_get_uvs(tile), (int)level_width);
    }

    shader.bind();

    glBufferData(GL_ARRAY_BUFFER, verts.verts.size() * (size)sizeof(WorldVertex),
                 verts.verts.data(), GL_STATIC_DRAW);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, verts.indices.size() * (size)sizeof(u32),
                 verts.indices.data(), GL_STATIC_DRAW);

    shader.unbind();
}

void set_cube(World& world, Cube& c, Lattice& l, u32 i, ivec2 pos, TileUV uv, int wsize) {
    c.reset(uv, i, wsize);

    TILE tile = world.grid[i];
    u8 walls = nbs_walls(&world->grid, pos);

    if (tile == TILE::ROCK_GROUND || tile == TILE::BRICK_GROUND || tile == TILE::WATER_GROUND) {
        cube_on(c, FACE_DIR::GROUND);
    } else {
        cube_on(c, FACE_DIR::TOP);

        if (!nbs_up(walls)) {
            cube_on(c, FACE_DIR::BACK);
            if (pos[1] == 0) {
                c->verts[4].color = 0;
                c->verts[7].color = 0;
            }
        }
        if (!nbs_right(walls)) {
            cube_on(c, FACE_DIR::RIGHT);
            if (pos[0] == WSIZE - 1) {
                c->verts[12].color = 0;
                c->verts[15].color = 0;
            }
        }
        if (!nbs_down(walls)) {
            cube_on(c, FACE_DIR_FRONT);
            if (pos[1] == WSIZE - 1) {
                c->verts[0].color = 0;
                c->verts[3].color = 0;
            }
        }
        if (!nbs_left(walls)) {
            cube_on(c, FACE_DIR_LEFT);
            if (pos[0] == 0) {
                c->verts[8].color = 0;
                c->verts[11].color = 0;
            }
        }
    }

    l.from_world_vertices(l, c->verts, true);

    const float y_offset = 0.0f;
    lattice_add_slope_perlin(l, V3((float)pos[0], 0, (float)pos[1]), y_offset,
                             PERLIN_DEFAULT_STRENGTH);

    u8 water = nbs_water(&world->grid, pos);

    const float WATER_DEPTH = WATER_LEVEL * 2.4f;
    if (tile == TILE_WATER_GROUND) {
        lattice_add_offset(l, WATER_DEPTH);
    } else {
        lattice_match_liquid_level(l, water, WATER_DEPTH);
    }

    lattice_apply_to_cube(l, c);

    if (tile == TILE_WATER_GROUND) {
        // Make water corners rounded.
        cube_is_water(c, walls, water, 0.1f);
    } else if (water > 0) {
        // Meet rounded corners where they exist.
        // Add shadows.
        cube_is_adjacent_to_water(c, walls, water, 0.1f);
    }

    cube_shade_unshaded_corners(c, walls);

    cube_fix_nbs_wall_uvs(c, world, walls);

    cube_add_verts_and_indices(c, &world->verts);
}
