#include "meshes.hpp"
#include "mesh.hpp"
#include <stdexcept>

void Meshes::init() {
    meshes[MESH::SKAL] = mesh_for_type(MESH::SKAL);
}

Mesh Meshes::get(MESH type) {
    if (auto result = meshes.find(type); result != meshes.end()) {
        return result->second;
    } else {
        throw std::runtime_error(frmt("Missing mesh for type %d", type));
    }
}

Mesh &Meshes::get_ref(MESH type) {
    if (auto result = meshes.find(type); result != meshes.end()) {
        return result->second;
    } else {
        throw std::runtime_error(frmt("Missing mesh for type %d", type));
    }
}
