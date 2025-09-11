#pragma once

#include "animator.hpp"

struct Level;

enum struct ANIMATION : u8 {
    SQUATS
};

struct Animations {
    std::unordered_map<ANIMATION, Animation> animations;

    void init(Level &);
    Animation* get(ANIMATION);
};
