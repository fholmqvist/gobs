#pragma once

#include "bounds.hpp"
#include "light.hpp"
#include "storage.hpp"

struct Lights {
    Storage<Light> lights;

    void add(Light l);
    LightUBO pack(std::vector<Light>);
    std::vector<Light> build(SphereBound bounds);
};
