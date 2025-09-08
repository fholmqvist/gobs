#pragma once

#include "input.hpp"

struct Game {
    void init();
    void render();
    void update();

    bool running;

  private:
    Input input;
};
