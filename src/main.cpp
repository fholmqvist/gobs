#include "game.hpp"

Game game;

int main() {
    if (!game.init()) {
        throw std::runtime_error("Game failed to initialize");
    }

    Level l = Level();

    while (game.running) {
        game.update(l);
        game.render(l);
    }

    return 0;
}
