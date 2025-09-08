#pragma once

#include "base.hpp"
#include "gui.hpp"
#include "input.hpp"
#include "level.hpp"
#include "world.hpp"

struct Game {
    bool init();
    void update(Level &l);
    void render(Level &l);

    World world;
    Level level;

    bool running;

  private:
    Input input;
    GUI gui;

    bool DISABLE_SHADERS;
    SDL_Window* WINDOW;
    SDL_GLContext GL_CONTEXT;

    void load_assets();
    void load_texture(std::string path, u32* texture_id, int color_format);
};
