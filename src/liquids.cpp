#include "liquids.hpp"

ID Liquids::add(World &world, LIQUID type, ivec4 pos) {
    world.set_square(pos, TILE::WATER_GROUND);
    Liquid lq(type, (ivec4){ pos[0] - 1, pos[1] - 1, pos[2] + 1, pos[3] + 1 }, world.level_width);
    liquids.emplace_back(lq);
    return lq.id;
}

void Liquids::update() {
    for (auto liquid : liquids) {
        liquid.update();
    }
}

void Liquids::render() {
}
