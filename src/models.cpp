#include "models.hpp"

#include "camera.hpp"
#include "level.hpp"
#include "sample_grid.hpp"

Shader model_shader(
    "assets/shaders/model_vs.glsl", "assets/shaders/model_fs.glsl",
    [](Shader &s) {
        GLsizei stride = sizeof(BoneVertex);

        GLuint block = glGetUniformBlockIndex(s.ID, "Lights");
        glUniformBlockBinding(s.ID, block, 0);

        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(BoneVertex, pos));

        glEnableVertexAttribArray(1); // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(BoneVertex, norm));

        glEnableVertexAttribArray(2); // uv
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(BoneVertex, uv));

        glEnableVertexAttribArray(3); // vcolor
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(BoneVertex, color));

        glEnableVertexAttribArray(4); // bone indexes
        glVertexAttribIPointer(4, 4, GL_BYTE, stride, (void*)offsetof(BoneVertex, bone_indexes));

        glEnableVertexAttribArray(5); // bone weights
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, stride,
                              (void*)offsetof(BoneVertex, bone_weights));
    },
    [](Shader &s, Level &l) {
        glCullFace(GL_BACK);

        int view_projection = glGetUniformLocation(s.ID, "view_projection");
        glUniformMatrix4fv(view_projection, 1, GL_FALSE,
                           value_ptr(PERSPECTIVE * CAMERA->view_matrix()));

        for (auto &model : l.systems.models.models.values) {
            Mesh mesh = model.mesh;

            {
                SphereBound obj_bound;
                obj_bound.radius = 20.0f;
                obj_bound.pos = model.pos;

                std::vector<Light> lights = l.systems.lights.build(obj_bound);
                LightUBO pack = l.systems.lights.pack(lights);

                glBindBuffer(GL_UNIFORM_BUFFER, s.UBO);
                usize bytes = sizeof(pack.header) + (usize)lights.size() * 12 * sizeof(float);
                glBufferSubData(GL_UNIFORM_BUFFER, 0, (size)bytes, &pack);
            }

            glBufferData(GL_ARRAY_BUFFER, mesh.verts.size() * (size)sizeof(BoneVertex),
                         mesh.verts.data(), GL_STATIC_DRAW);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * (size)sizeof(u16),
                         mesh.indices.data(), GL_STATIC_DRAW);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, mesh.texture_id);
            glUniform1i(glGetUniformLocation(s.ID, "_texture"), 0);

            int world_pos = glGetUniformLocation(s.ID, "world_pos");
            glUniform3f(world_pos, model.pos[0], model.pos[1], model.pos[2]);
            int rotation = glGetUniformLocation(s.ID, "rotation");
            glUniform3f(rotation, model.rot[0], model.rot[1], model.rot[2]);
            int scale = glGetUniformLocation(s.ID, "scale");
            glUniform1f(scale, model.scale);

            int final_bone_matrices_pos = glGetUniformLocation(s.ID, "final_bone_matrices");
            glUniformMatrix4fv(final_bone_matrices_pos,
                               (GLsizei)model.animator.final_bone_matrices.size(), GL_FALSE,
                               (const GLfloat*)value_ptr(model.animator.final_bone_matrices[0]));

            glDrawElements(GL_TRIANGLES, (GLsizei)mesh.indices.size(), GL_UNSIGNED_SHORT, 0);
        }

        glCullFace(GL_FRONT);
    });

void Models::init(Animations& animations) {
    meshes.init(animations);
}

void Models::update() {
    for (auto &m : models.values) {
        m.update();
    }
}

GID Models::add(std::vector<WorldVertex> &grid, MESH type, vec3 pos, vec3 rot, float scale,
                int wsize) {
    Model model(meshes.get(type), type, pos, rot, scale);

    vec3 normal;
    float avg_height = sample_floor_y(grid, pos[0], pos[2], wsize, normal);
    float floor_y = avg_height;
    model.move(vec2(0), floor_y, normal);

    return models.add(model);
}
