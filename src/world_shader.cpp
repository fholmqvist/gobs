#include "world_shader.hpp"

#include "camera.hpp"
#include "constants.hpp"
#include "cube.hpp"

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
