#include "camera.hpp"
#include "game.hpp"

int main() {
    Game game;

    if (!game.init()) {
        throw std::runtime_error("Game failed to initialize");
    }

    game.level = Level(8);
    game.level.init([&game](Level &l) {
        CAMERA->pos = vec3{ 3.5, 7, 8 };
        game.world.init(l);
        game.world.set_square({ 1, 1, 7, 7 }, TILE::BRICK_GROUND);
        game.world.reset_opengl(l);
    });

    game.world.set({ 3, 3 }, TILE::WATER_GROUND);

    while (game.running) {
        game.update();
        game.render();
    }

    return 0;
}
