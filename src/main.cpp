#include "base.hpp"
#include "game.hpp"
#include "levels.hpp"
#include "timer.hpp"

int main() {
    log_info("Starting game");

    Timer timer;
    Window window;
    if (!window.init()) {
        throw std::runtime_error("Window failed to initialize");
    }

    Game game(debug_level());

    if (!game.init()) {
        throw std::runtime_error("Game failed to initialize");
    }

    log_info("Game started in %s", time_to_string(timer.stop()).c_str());

    while (game.running) {
        game.update();
        game.render(window);
    }

    return 0;
}
