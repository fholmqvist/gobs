#pragma once

#include "base.hpp"
#include "gui.hpp"
#include "input.hpp"
#include "level.hpp"

struct Game {
    bool init();
    void update(Level& l);
    void render(Level& l);

    bool running;

  private:
    Input input;
    GUI gui;

    bool DISABLE_SHADERS;
    SDL_Window* WINDOW;
    SDL_GLContext GL_CONTEXT;

    Level level;

    void load_assets();
    void load_texture(std::string path, u32* texture_id, int color_format);
};
