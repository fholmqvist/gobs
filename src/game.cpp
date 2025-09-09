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

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    const auto ar = aspect_ratio(SCREEN_W, SCREEN_H);
    if (ar.first != 4 || ar.second != 3) {
        throw std::runtime_error(frmt("Illegal screen resolution %d:%d", ar.first, ar.second));
    }

    WINDOW = SDL_CreateWindow("GOBS", SCREEN_W, SCREEN_H, SDL_WINDOW_OPENGL);
    if (!WINDOW) {
        SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return false;
    }

    GL_CONTEXT = SDL_GL_CreateContext(WINDOW);
    if (!GL_CONTEXT) {
        SDL_Log("SDL_GL_CreateContext Error: %s\n", SDL_GetError());
        return false;
    }

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "GLEW init failed: %s\n", glewGetErrorString(err));
        return false;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    SDL_zero(input.e);

    glClearColor(0, 0, 0, 1);

    // VSYNC.
    SDL_GL_SetSwapInterval(1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    log_info("Initialized SDL in %s", time_to_string(GLOBAL_TIMER.stop()).c_str());

    load_assets();

    systems.world.shader.init();
    gui.init();

    running = true;
    return true;
}

void Game::update() {
    GLOBAL_TIMER.reset();

    running = input.update(systems.level.width);
    systems.level.update();
    systems.liquids.update();
    gui.update();
    STATS.update(GLOBAL_TIMER.stop());
}

void Game::render() {
    GLOBAL_TIMER.reset();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    systems.world.shader.render(systems.level);
    systems.liquids.render();
    gui.render();

    SDL_GL_SwapWindow(WINDOW);

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

    load_texture("assets/textures/bones.png", &SKAL_TEXTURE, GL_RGB);
    load_texture("assets/textures/gob.png", &GOB_TEXTURE, GL_RGB);

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
