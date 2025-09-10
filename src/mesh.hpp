#pragma once

#include "bone.hpp"
#include "vertices.hpp"
#include <assimp/scene.h>

enum struct MESH : u8 {
    MESH_UNKNOWN,
    SKAL,
};

struct Mesh {
    std::vector<BoneVertex> verts;
    std::vector<i16> indices;
    u32 texture_id;

    // TODO: Expensive to duplicate, extract.
    std::unordered_map<std::string, BoneInfo> table;

    Mesh for_type(MESH);
    static Mesh from_scene_node(MESH, const aiScene*, const aiNode*);
};
