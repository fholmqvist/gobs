#pragma once

#include "animation.hpp"

struct Animator {
    float curr_time;
    Animation* curr_anim;

    std::vector<mat4> final_bone_matrices;

    Animator();
    void update();
    void play(Animation*);
    void bone_transforms(AssimpNodeData* node, mat4 parent);
};
