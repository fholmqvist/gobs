#include "camera.hpp"
#include "game.hpp"

int main() {
    Game game;

    if (!game.init()) {
        throw std::runtime_error("Game failed to initialize");
    }

    game.level = Level(8);
    game.level.init([](Level &l) {
        CAMERA->pos = vec3{ 3.5, 7, 8 };
        l.world.set_square({ 1, 1, 7, 7 }, TILE::BRICK_GROUND);
        l.add_liquid(LIQUID::OIL, { 3, 3, 5, 5 });
        l.world.reset_opengl(l);
    });

    while (game.running) {
        game.update();
        game.render();
    }

    return 0;
}
