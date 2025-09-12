#pragma once

#include "mesh.hpp"

struct Animations;

struct Meshes {
    std::unordered_map<MESH, Mesh> meshes;

    void init(Animations& animations);
    Mesh get(MESH);
    Mesh& get_ref(MESH);
};
