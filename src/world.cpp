#include "world.hpp"
#include "cube.hpp"

Shader world_shader =
    Shader("assets/shaders/grid.vs", "assets/shaders/grid.fs", [](Shader &s, Level &level) {
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
