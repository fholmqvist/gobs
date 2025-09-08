#include "game.hpp"

#include <stdexcept>

Game game;

int main() {
    if (!game.init()) {
        throw std::runtime_error("Game failed to initialize");
    }

    while (game.running) {
        game.render();
        game.update();
    }

    return 0;
}
