#include "meshes.hpp"
#include "mesh.hpp"

void Meshes::init(Animations& animations) {
    meshes[MESH::GOB] = mesh_for_type(MESH::GOB, animations);
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
