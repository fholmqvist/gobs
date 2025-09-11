#pragma once

#include "mesh.hpp"

struct Meshes {
    std::unordered_map<MESH, Mesh> meshes;

    void init();
    Mesh get(MESH);
    Mesh& get_ref(MESH);
};
