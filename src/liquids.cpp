#include "liquids.hpp"

ID Liquids::add(LIQUID type, ivec4 pos, int wsize) {
    Liquid lq(type, (ivec4){ pos[0] - 1, pos[1] - 1, pos[2] + 1, pos[3] + 1 }, wsize);
    liquids.emplace_back(lq);
    return lq.id;
}

void Liquids::update() {
    for (auto liquid : liquids) {
        liquid.update();
    }
}

// TODO: Liquids shader.
void Liquids::render() {
}
