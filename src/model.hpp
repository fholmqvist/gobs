#pragma once

#include "animator.hpp"
#include "mesh.hpp"

struct Model {
    Mesh mesh;
    MESH type;
    vec3 pos;
    vec3 rot;
    float scale;

    Animator animator;

    Model(MESH, vec3 pos, vec3 rot, float scale);
    void update();
    void move(vec2 diff, float floor_y, vec3 floor_norm);
};
