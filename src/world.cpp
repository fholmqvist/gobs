#include "world.hpp"
#include "cube.hpp"
#include "ivec2.hpp"
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

Tile World::get(ivec2 pos) {
    return grid[ivec2_to_idx(pos, level_width)];
}

void World::reset_opengl(Level &l) {
    verts.verts.clear();
    verts.verts.reserve(l.total * N_VERTS_PER_CUBE);
    verts.indices.clear();
    verts.indices.reserve(l.total * CUBE_INDICES);

    for (u32 i = 0; i < l.total; i++) {
        ivec2 pos = ivec2_from_idx(i, l.width);
        Tile tile = get(pos);

        // set_cube(cube, lattice, i, pos, tile_get_uvs(tile));
    }

    shader.bind();

    glBufferData(GL_ARRAY_BUFFER, verts.verts.size() * (size)sizeof(WorldVertex),
                 verts.verts.data(), GL_STATIC_DRAW);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, verts.indices.size() * (size)sizeof(u32),
                 verts.indices.data(), GL_STATIC_DRAW);

    shader.unbind();
}
