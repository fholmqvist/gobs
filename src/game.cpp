#include "base.hpp"
#include "pch.hpp"

#include "constants.hpp"
#include "game.hpp"
#include "stats.hpp"
#include "timer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

bool Game::init() {
    GLOBAL_TIMER.reset();

    SDL_zero(input.e);

    load_assets();

    store_glsl_helper("assets/shaders/helpers.glsl");

    gui.init();
    level.init();

    running = true;
    return true;
}

void Game::update() {
    GLOBAL_TIMER.reset();

    running = input.update(level.width);
    level.update();
    gui.update();
    STATS.update(GLOBAL_TIMER.stop());
}

void Game::render(Window &window) {
    GLOBAL_TIMER.reset();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    level.render();
    gui.render();

    SDL_GL_SwapWindow(window.WINDOW);

    STATS.add_render_time(GLOBAL_TIMER.stop());
}

void Game::load_assets() {
    GLOBAL_TIMER.reset();

    load_texture("assets/textures/font.png", &FONT_TEXTURE, GL_RGBA);

    load_texture("assets/textures/tilemap.png", &TILE_MAP_TEXTURE, GL_RGB);
    load_texture("assets/textures/water.png", &WATER_TEXTURE, GL_RGB);
    load_texture("assets/textures/rail.png", &RAIL_TEXTURE, GL_RGB);
    load_texture("assets/textures/rail_cart.png", &RAIL_CART_TEXTURE, GL_RGB);
    load_texture("assets/textures/pipes.psd", &PIPE_TEXTURE, GL_RGBA);

    load_texture("assets/textures/gob.psd", &GOB_TEXTURE, GL_RGBA);

    log_info("Loaded assets in %s", time_to_string(GLOBAL_TIMER.stop()).c_str());
}

void Game::load_texture(std::string path, u32* texture_id, int color_format) {
    int width, height, nr_channels;
    stbi_uc* texture_data = stbi_load(path.c_str(), &width, &height, &nr_channels, 0);
    if (!texture_data) {
        throw std::runtime_error("Failed to load texture " + path);
        return;
    }

    glGenTextures(1, texture_id);
    glBindTexture(GL_TEXTURE_2D, *texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, color_format, width, height, 0, (unsigned int)color_format,
                 GL_UNSIGNED_BYTE, texture_data);

    stbi_image_free(texture_data);
}
