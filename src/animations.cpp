#include "animations.hpp"

#include "level.hpp"

void Animations::init(Level &l) {
    animations[ANIMATION::SQUATS] =
        Animation("assets/models/bones.fbx", l.systems.models.meshes.get_ref(MESH::SKAL));
}

Animation* Animations::get(ANIMATION anim) {
    if (auto result = animations.find(anim); result != animations.end()) {
        return &result->second;
    } else {
        throw std::runtime_error(frmt("Missing animations type %d", anim));
    }
}
