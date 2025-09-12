#pragma once

#include "animator.hpp"

struct Level;

enum struct ANIMATION : u8 {
    IDLE
};

struct Animations {
    std::unordered_map<ANIMATION, Animation> animations;

    void add(ANIMATION, Animation);
    Animation* get(ANIMATION);
};

ANIMATION anim_from_string(std::string str);
