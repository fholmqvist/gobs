#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "base.hpp"

struct KeyPos {
    vec3 pos;
    float timestamp;
};

struct KeyRot {
    quat orientation;
    float timestamp;
};

struct KeyScale {
    vec3 scale;
    float timestamp;
};

struct Bone {
    size bone_index;
    std::string name;

    std::vector<KeyPos> pos;
    std::vector<KeyRot> rot;
    std::vector<KeyScale> scale;

    mat4 local_transform;

    Bone(std::string name, size index, aiNodeAnim &channel);

    void update(float time);
};
