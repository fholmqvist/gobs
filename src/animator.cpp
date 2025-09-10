#include "animator.hpp"

#include "stats.hpp"

Animator::Animator() {
    curr_time = 0.0f;
    curr_anim = nullptr;

    final_bone_matrices.reserve(32);
    for (size i = 0; i < 32; i++) {
        final_bone_matrices.push_back(mat4(1));
    }
}

void Animator::play(Animation* a) {
    curr_anim = a;
    curr_time = 0.0f;
}

void Animator::update() {
    if (!curr_anim) {
        return;
    }
    curr_time += curr_anim->tps * DELTA_TIME;
    curr_time = (float)fmod(curr_time, curr_anim->duration);
    bone_transforms(&curr_anim->root_node, mat4(1));
}

void Animator::bone_transforms(AssimpNodeData* node, mat4 parent) {
    if (node->name.empty()) {
        return;
    }

    auto node_transform = node->transform;

    Bone* bone = curr_anim->find_bone(node->name);
    if (bone) {
        bone->update(curr_time);
        node_transform = bone->local_transform;
    }

    auto global_transform = parent * node_transform;

    if (auto bi = curr_anim->table.find(node->name); bi != curr_anim->table.end()) {
        if (bi->second.index < (size)final_bone_matrices.size()) {
            mat4 tmp = curr_anim->global_inverse * global_transform;
            mat4 final = tmp * bi->second.offset;

            final_bone_matrices[bi->second.index] = final;
        }
    }

    for (auto &child : node->children) {
        bone_transforms(&child, global_transform);
    }
}
