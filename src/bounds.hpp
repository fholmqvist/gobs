#pragma once

#include "light.hpp"

struct SphereBound {
    vec3 pos;
    float radius;

    static SphereBound from_point_light(Light p);
    bool intersect(SphereBound);
};
