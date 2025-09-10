#pragma once

#include "pch.hpp" // IWYU pragma: keep

struct Vertex {
    vec3 pos;
    vec3 norm;
    vec2 uv;
    float color;
};

struct WorldVertex {
    vec3 pos;
    vec3 norm;
    vec2 uv;
    vec2 wpos;
    float color;
};

#define BONE_MAX_PER_VERTEX 4

struct BoneVertex {
    vec3 pos;
    vec3 norm;
    vec2 uv;
    float color;
    std::array<i8, BONE_MAX_PER_VERTEX> bone_indexes;
    std::array<float, BONE_MAX_PER_VERTEX> bone_weights;
};
