#include "systems.hpp"

void Systems::init(Level &l) {
    world.init(l);
    liquids.init();
}
