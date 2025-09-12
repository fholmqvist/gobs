#include "levels.hpp"

#include "camera.hpp"

LevelF debug_level = []() {
    return Level(8, [](Level &l) {
        CAMERA->pos = vec3{ 3.5, 7, 8 };
        l.systems.world.set_square({ 1, 1, 7, 7 }, TILE::BRICK_GROUND);
        l.add_liquid(LIQUID::WATER, { 3, 3, 5, 5 });
        l.systems.world.reset_opengl(l);
        l.add_light({ { 1.5, 1, 1.5 }, { 1, 0, 0 }, 4, 4 });
        l.add_light({ { 5.5, 1, 1.5 }, { 0, 1, 0 }, 4, 4 });
        l.add_light({ { 1.5, 1, 5.5 }, { 1, 1, 0 }, 4, 4 });
        l.add_light({ { 5.5, 1, 5.5 }, { 0, 0, 1 }, 4, 4 });
        auto id = l.add_model(MESH::GOB, { 1, 1, 1 }, vec3(0), 1.0);
        l.play_animation(id, ANIMATION::WALK);
    });
};
