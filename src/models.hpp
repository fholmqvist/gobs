#pragma once

#include "camera.hpp"
#include "lights.hpp"
#include "model.hpp"
#include "shader.hpp"
#include "storage.hpp"
#include "vertices.hpp"

struct Models {
    Shader s;
    Storage<Model> ms;

    void render(Camera &, Lights &);
    void update();
    GID add(std::vector<WorldVertex> &grid, MESH, vec3 pos, vec3 rot, float scale, int wsize);
};
