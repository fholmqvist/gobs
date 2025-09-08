#include "game.hpp"

int main() {
    Game game;

    Level l = Level();

    if (!game.init(l)) {
        throw std::runtime_error("Game failed to initialize");
    }

    while (game.running) {
        game.update(l);
        game.render(l);
    }

    return 0;
}
