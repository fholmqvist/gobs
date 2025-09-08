#include "game.hpp"

int main() {
    Game game;

    if (!game.init()) {
        throw std::runtime_error("Game failed to initialize");
    }

    Level l = Level();
    l.init([&game](Level &l) {
        l.width = 8;
        l.total = l.width * l.width;
        game.world.init(l);
        game.world.set(ivec2{ 1, 1 }, TILE::ROCK_GROUND);
        game.world.reset_opengl(l);
    });

    while (game.running) {
        game.update(l);
        game.render(l);
    }

    return 0;
}
