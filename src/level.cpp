#include "level.hpp"

#include "base.hpp"
#include "timer.hpp"

void Level::init() {
    assert(width > 0);

    GLOBAL_TIMER.reset();

    systems.world.init(*this);
    systems.liquids.init();

    initf(*this);

    log_info("Initialized level in %s", GLOBAL_TIMER.stop_string().c_str());
}

void Level::update() {
    systems.liquids.update();
}

void Level::render() {
    systems.world.shader.render(*this);
    systems.liquids.render(*this);
}

void Level::add_liquid(LIQUID type, ivec4 pos) {
    systems.world.set_square(pos, TILE::WATER_GROUND);
    systems.liquids.add(type, pos, width);
}
