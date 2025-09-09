#include "level.hpp"

#include "base.hpp"
#include "timer.hpp"

void Level::init(std::function<void(Level &l)> f) {
    assert(width > 0);

    GLOBAL_TIMER.reset();

    world.init(*this);
    world.shader.init();

    f(*this);

    log_info("Initialized level in %s", GLOBAL_TIMER.stop_string().c_str());
}

void Level::update() {
    liquids.update();
}

void Level::render() {
    world.shader.render(*this);
    liquids.render();
}

void Level::add_liquid(LIQUID type, ivec4 pos) {
    world.set_square(pos, TILE::WATER_GROUND);
    liquids.add(type, pos, width);
}
