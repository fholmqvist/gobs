#include "game.hpp"

int main() {
    Game game;

    if (!game.init()) {
        throw std::runtime_error("Game failed to initialize");
    }

    Level l = Level();
    l.init([&game](Level &l) {
        l.width = 8;
        game.world.reset_opengl(l);
    });

    while (game.running) {
        game.update(l);
        game.render(l);
    }

    return 0;
}
