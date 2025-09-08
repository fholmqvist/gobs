#include "level.hpp"

#include "base.hpp"
#include "timer.hpp"

void Level::init(std::function<void(Level &l)> f) {
    GLOBAL_TIMER.reset();
    f(*this);
    log_info("Level initialized in %s", GLOBAL_TIMER.stop_string().c_str());
}

void Level::update() {
}

void Level::render() {
}
