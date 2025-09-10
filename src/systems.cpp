#include "systems.hpp"

#include "level.hpp"

void Systems::init(Level &l) {
    world.init(l);
    liquids.init();
    models.shader.init();
}

void Systems::update() {
    liquids.update();
}

void Systems::render(Level &l) {
    world.shader.render(l);
    liquids.render(l);
    models.shader.render(l);
}
