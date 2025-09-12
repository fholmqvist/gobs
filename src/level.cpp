#include "level.hpp"

#include "animations.hpp"
#include "base.hpp"
#include "timer.hpp"

void Level::init() {
    assert(width > 0);

    GLOBAL_TIMER.reset();

    systems.init(*this);

    initf(*this);

    log_info("Initialized level in %s", GLOBAL_TIMER.stop_string().c_str());
}

void Level::update() {
    systems.update();
}

void Level::render() {
    systems.render(*this);
}

GID Level::add_liquid(LIQUID type, ivec4 pos) {
    systems.world.set_square(pos, TILE::WATER_GROUND);
    return systems.liquids.add(type, pos, width);
}

GID Level::add_light(Light light) {
    return systems.lights.add(light);
}

GID Level::add_model(MESH type, vec3 pos, vec3 rot, float scale) {
    return systems.models.add(systems.world.verts.verts, type, pos, rot, scale, width);
}

void Level::play_animation(GID id, ANIMATION anim) {
    Model* m = systems.models.get(id);
    if (!m) {
        return;
    }
    m->animator.play(systems.animations.get(anim));
}
