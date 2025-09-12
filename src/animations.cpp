#include "animations.hpp"

void Animations::add(ANIMATION type, Animation anim) {
    animations[type] = anim;
}

Animation* Animations::get(ANIMATION anim) {
    if (auto result = animations.find(anim); result != animations.end()) {
        return &result->second;
    } else {
        throw std::runtime_error(frmt("Missing animations type %d", anim));
    }
}
