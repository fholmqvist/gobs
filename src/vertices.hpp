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
