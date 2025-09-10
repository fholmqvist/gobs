#include "camera.hpp"
#include "game.hpp"

int main() {
    Window window;
    if (!window.init()) {
        throw std::runtime_error("Window failed to initialize");
    }

    Game game(Level(8, [](Level &l) {
        CAMERA->pos = vec3{ 3.5, 7, 8 };
        l.systems.world.set_square({ 1, 1, 7, 7 }, TILE::BRICK_GROUND);
        l.add_liquid(LIQUID::WATER, { 3, 3, 5, 5 });
        l.systems.world.reset_opengl(l);
        l.systems.lights.add({ { 1, 1, 1 }, { 1, 0, 0 }, 4, 4 });
        l.systems.lights.add({ { 7, 1, 1 }, { 0, 1, 0 }, 4, 4 });
        l.systems.lights.add({ { 1, 1, 7 }, { 1, 1, 0 }, 4, 4 });
        l.systems.lights.add({ { 7, 1, 7 }, { 0, 0, 1 }, 4, 4 });
    }));

    if (!game.init()) {
        throw std::runtime_error("Game failed to initialize");
    }

    while (game.running) {
        game.update();
        game.render(window);
    }

    return 0;
}
