#include "liquids.hpp"

#include "camera.hpp"
#include "constants.hpp"
#include "cube.hpp"
#include "level.hpp"
#include "stats.hpp"

static float x_offset = 0.0f;
static float y_offset = 0.0f;

Shader liquid_shader(
    "assets/shaders/liquid_vs.glsl", "assets/shaders/liquid_fs.glsl",
    [](Shader &s) {
        (void)s;

        GLsizei stride = VERT_STRIDE;

        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(WorldVertex, pos));

        glEnableVertexAttribArray(1); // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(WorldVertex, norm));

        glEnableVertexAttribArray(2); // uv
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(WorldVertex, uv));

        glEnableVertexAttribArray(3); // wpos
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(WorldVertex, wpos));

        glEnableVertexAttribArray(4); // color
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, stride,
                              (void*)offsetof(WorldVertex, color));
    },
    [](Shader &s, Level &l) {
        int view_projection = glGetUniformLocation(s.ID, "view_projection");
        glUniformMatrix4fv(view_projection, 1, GL_FALSE,
                           value_ptr(PERSPECTIVE * CAMERA->view_matrix()));

        int scroll = glGetUniformLocation(s.ID, "scroll");
        glUniform2f(scroll, x_offset, y_offset);
        x_offset += (DELTA_TIME * 0.7f) / 128.0f;
        y_offset += (DELTA_TIME * 0.7f) / 128.0f;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (auto lq : l.systems.liquids.liquids) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, WATER_TEXTURE);
            glUniform1i(glGetUniformLocation(s.ID, "_texture"), 0);

            glUniform1f(glGetUniformLocation(s.ID, "_transparency"),
                        lq.type == LIQUID::WATER ? 0.32f : 0.98f);

            glBufferData(GL_ARRAY_BUFFER, lq.verts.verts.size() * (size)sizeof(WorldVertex),
                         lq.verts.verts.data(), GL_STATIC_DRAW);

            glBufferData(GL_ELEMENT_ARRAY_BUFFER, lq.verts.indices.size() * (size)sizeof(u32),
                         lq.verts.indices.data(), GL_STATIC_DRAW);

            glDrawElements(GL_TRIANGLES, (GLsizei)lq.verts.indices.size(), GL_UNSIGNED_INT, 0);
        }
        glDisable(GL_BLEND);
    });

void Liquids::init() {
    shader.init();
}

ID Liquids::add(LIQUID type, ivec4 pos, int wsize) {
    Liquid lq(type, (ivec4){ pos[0] - 1, pos[1] - 1, pos[2] + 1, pos[3] + 1 }, wsize);
    liquids.emplace_back(lq);
    return lq.id;
}

void Liquids::update() {
    for (auto liquid : liquids) {
        liquid.update();
    }
}

// TODO: Liquids shader.
void Liquids::render(Level &l) {
    shader.render(l);
}
