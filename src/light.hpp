#pragma once

#include "pch.hpp" // IWYU pragma: keep

#define LIGHTS_MAX 8u

struct Light {
    vec3 pos;
    vec3 color;
    float intensity;
    float radius;
};

typedef struct {
    std::array<float, 4> header;
    std::array<float, LIGHTS_MAX * 12> data;
} LightUBO;
