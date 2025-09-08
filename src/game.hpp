#pragma once

#include "base.hpp"
#include "input.hpp"
#include <string>

struct Game {
    bool init();
    void render();
    void update();

    bool running;

  private:
    Input input;

    void load_assets();
    void load_texture(std::string path, u32* texture_id, int color_format);
};
