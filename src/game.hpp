#pragma once

#include "base.hpp"
#include "input.hpp"
#include "level.hpp"

struct Game {
    bool init();
    void render();
    void update();

    bool running;

  private:
    Input input;

    const int SCREEN_W = 1200; // 640;
    const int SCREEN_H = 900;  // 480;

    bool DISABLE_SHADERS;
    SDL_Window* WINDOW;
    SDL_GLContext GL_CONTEXT;

    Level level;

    void load_assets();
    void load_texture(std::string path, u32* texture_id, int color_format);
};
