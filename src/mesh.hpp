#pragma once

#include "bone.hpp"
#include "vertices.hpp"

enum struct MESH : u8 {
    MESH_UNKNOWN,
    GOB,
};

struct Animations;

struct Mesh {
    std::vector<BoneVertex> verts;
    std::vector<i16> indices;
    u32 texture_id;

    // TODO: Expensive to duplicate, extract.
    std::unordered_map<std::string, BoneInfo> table;
};

Mesh mesh_for_type(MESH, Animations &);
Mesh mesh_from_scene_node(MESH, const aiScene*, Animations&, const aiNode*);
