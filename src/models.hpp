#pragma once

#include "meshes.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "storage.hpp"
#include "vertices.hpp"

extern Shader model_shader;

struct Models {
    Shader shader;
    Storage<Model> models;
    Meshes meshes;

    Models() : shader(model_shader) {};

    void init();
    void update();
    GID add(std::vector<WorldVertex> &grid, MESH, vec3 pos, vec3 rot, float scale, int wsize);
};
