#include "game.hpp"

void Game::init() {
    running = true;
}

void Game::render() {
}

void Game::update() {
    running = input.update();
}
