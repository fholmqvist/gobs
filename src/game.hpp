#pragma once

#include "base.hpp"
#include "gui.hpp"
#include "input.hpp"
#include "level.hpp"
#include "window.hpp"

struct Game {
    bool init();
    void update();
    void render(Window &);

    Game(Level l) : level(l) {};

    Level level;

    bool running;

  private:
    Input input;
    GUI gui;

    void load_assets();
    void load_texture(std::string path, u32* texture_id, int color_format);
};
