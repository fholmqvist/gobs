#include "world.hpp"

#include "camera.hpp"
#include "constants.hpp"
#include "cube.hpp"
#include "ivec2.hpp"
#include "lattice.hpp"
#include "level.hpp"
#include "neighbors.hpp"
#include "random.hpp"

Shader world_shader = Shader(
    "assets/shaders/grid_vs.glsl", "assets/shaders/grid_fs.glsl",
    [](Shader &s) {
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
    },
    [](Shader &s, Level &l) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TILE_MAP_TEXTURE);
        glUniform1i(glGetUniformLocation(s.ID, "_texture"), 0);

        int view_projection = glGetUniformLocation(s.ID, "view_projection");
        glUniformMatrix4fv(view_projection, 1, GL_FALSE,
                           value_ptr(PERSPECTIVE * CAMERA->view_matrix()));

        glDrawElements(GL_TRIANGLES, (GLsizei)l.n_indices, GL_UNSIGNED_INT, 0);
    });

void World::init(Level &l) {
    level_width = l.width;
    grid = std::vector(l.total, TILE::ROCK);
    reset_opengl(l);
}

void World::set(ivec2 pos, TILE t) {
    grid[ivec2_to_idx(pos, level_width)] = t;
    update_opengl(pos);
}

void World::set_square(ivec4 pos, TILE t) {
    for (size y = pos.y; y < pos.w; y++) {
        for (size x = pos.x; x < pos.z; x++) {
            set({ x, y }, t);
        }
    }
}

TILE World::get(ivec2 pos) {
    return grid[ivec2_to_idx(pos, level_width)];
}

void add_cube(World &world, Cube &c, Lattice &l, u32 i, ivec2 pos, TileUV uv, int wsize);
void update_cube(World &world, Cube &c, Lattice &l, u32 i, ivec2 pos, TileUV uv, int wsize);

void World::update_opengl(ivec2 pos) {
    shader.bind();

    Cube cube;
    Lattice lattice;

    for (auto dir : directions) {
        auto npos = pos + dir;
        if (!in_range(npos, level_width)) {
            continue;
        }

        update_one_tile(npos);
    }

    update_one_tile(pos);

    shader.unbind();
}

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

        add_cube(*this, cube, lattice, i, pos, tile_get_uvs(tile), (int)level_width);
    }

    shader.bind();

    glBufferData(GL_ARRAY_BUFFER, verts.verts.size() * (size)sizeof(WorldVertex),
                 verts.verts.data(), GL_STATIC_DRAW);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, verts.indices.size() * (size)sizeof(u32),
                 verts.indices.data(), GL_STATIC_DRAW);

    l.n_indices = verts.indices.size();

    shader.unbind();
}

void set_cube(World &world, Cube &c, Lattice &l, u32 i, ivec2 pos, TileUV uv, int wsize) {
    c.reset(uv, i, wsize);

    TILE tile = world.grid[i];
    auto walls = Neighbors::check(world.grid, wsize, pos,
                                  [](TILE t) { return t == TILE::BRICK || t == TILE::ROCK; });

    if (tile == TILE::ROCK_GROUND || tile == TILE::BRICK_GROUND || tile == TILE::WATER_GROUND) {
        c.on(FACE_DIR::GROUND);
    } else {
        c.on(FACE_DIR::TOP);

        if (!walls.up()) {
            c.on(FACE_DIR::BACK);
            if (pos[1] == 0) {
                c.verts[4].color = 0;
                c.verts[7].color = 0;
            }
        }
        if (!walls.right()) {
            c.on(FACE_DIR::RIGHT);
            if (pos[0] == wsize - 1) {
                c.verts[12].color = 0;
                c.verts[15].color = 0;
            }
        }
        if (!walls.down()) {
            c.on(FACE_DIR::FRONT);
            if (pos[1] == wsize - 1) {
                c.verts[0].color = 0;
                c.verts[3].color = 0;
            }
        }
        if (!walls.left()) {
            c.on(FACE_DIR::LEFT);
            if (pos[0] == 0) {
                c.verts[8].color = 0;
                c.verts[11].color = 0;
            }
        }
    }

    l.from_world_vertices(c.verts, true);

    const float y_offset = 0.0f;
    l.add_slope_perlin(vec3{ pos[0], 0, pos[1] }, y_offset, PERLIN_DEFAULT_STRENGTH);

    auto water =
        Neighbors::check(world.grid, wsize, pos, [](TILE t) { return t == TILE::WATER_GROUND; });

    const float WATER_DEPTH = WATER_LEVEL * 2.4f;
    if (tile == TILE::WATER_GROUND) {
        l.add_offset(WATER_DEPTH);
    } else {
        l.match_liquid_level(water, WATER_DEPTH);
    }

    l.apply_to_cube(c);

    if (tile == TILE::WATER_GROUND) {
        // Make water corners rounded.
        c.is_water(walls, water, 0.1f);
    } else if (water.any()) {
        // Meet rounded corners where they exist.
        // Add shadows.
        c.is_adjacent_to_water(walls, water, 0.1f);
    }

    c.shade_unshaded_corners(walls);

    c.fix_nbs_wall_uvs(world, walls, wsize);
}

void World::update_one_tile(ivec2 pos) {
    size idx = ivec2_to_idx(pos, level_width);
    TILE tile = get(pos);

    Cube cube;
    Lattice lattice;

    update_cube(*this, cube, lattice, idx, pos, tile_get_uvs(tile), (int)level_width);

    glBufferSubData(GL_ARRAY_BUFFER,                              //
                    idx * N_VERTS_PER_CUBE * sizeof(WorldVertex), //
                    N_VERTS_PER_CUBE * sizeof(WorldVertex),       //
                    verts.verts.data() + idx * N_VERTS_PER_CUBE);

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER,       //
                    idx * N_INDICES * sizeof(u32), //
                    N_INDICES * sizeof(u32),       //
                    verts.indices.data() + idx * N_INDICES);
}

void add_cube(World &world, Cube &c, Lattice &l, u32 i, ivec2 pos, TileUV uv, int wsize) {
    set_cube(world, c, l, i, pos, uv, wsize);
    c.add_verts_and_indices(world.verts);
}

void update_cube(World &world, Cube &c, Lattice &l, u32 i, ivec2 pos, TileUV uv, int wsize) {
    set_cube(world, c, l, i, pos, uv, wsize);
    c.update_verts_and_indices(world.verts, i);
}
