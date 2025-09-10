#include "game.hpp"
#include "levels.hpp"

int main() {
    Window window;
    if (!window.init()) {
        throw std::runtime_error("Window failed to initialize");
    }

    Game game(debug_level());

    if (!game.init()) {
        throw std::runtime_error("Game failed to initialize");
    }

    while (game.running) {
        game.update();
        game.render(window);
    }

    return 0;
}
