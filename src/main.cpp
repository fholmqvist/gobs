#include "camera.hpp"
#include "game.hpp"

int main() {
    Game game;

    if (!game.init()) {
        throw std::runtime_error("Game failed to initialize");
    }

    Level l = Level(8);
    l.init([&game](Level &l) {
        CAMERA->pos = vec3{ 2, 4, 4.5 };
        game.world.init(l);
        game.world.set_square({ 1, 1, 3, 3 }, TILE::BRICK_GROUND);
        game.world.reset_opengl(l);
    });

    while (game.running) {
        game.update(l);
        game.render(l);
    }

    return 0;
}
