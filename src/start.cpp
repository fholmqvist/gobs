#include "start.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

#include "constants.hpp"

#include <GL/glew.h>

#include <GL/gl.h>
#include <stdexcept>

int start() {
    if (!init_SDL()) {
        throw std::runtime_error("SDL init failed");
    }
    load_assets();

    return 0;
}

bool init_SDL() {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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
        exit(1);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    SDL_zero(EVENT);

    // VSYNC.
    SDL_GL_SetSwapInterval(1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    return true;
}

void load_assets() {
    load_texture("assets/textures/font.png", &FONT_TEXTURE, GL_RGBA);

    load_texture("assets/textures/tilemap.png", &TILE_MAP_TEXTURE, GL_RGB);
    load_texture("assets/textures/water.png", &WATER_TEXTURE, GL_RGB);
    load_texture("assets/textures/rail.png", &RAIL_TEXTURE, GL_RGB);
    load_texture("assets/textures/rail_cart.png", &RAIL_CART_TEXTURE, GL_RGB);
    load_texture("assets/textures/pipes.psd", &PIPE_TEXTURE, GL_RGBA);

    load_texture("assets/textures/bones.png", &SKAL_TEXTURE, GL_RGB);
    load_texture("assets/textures/gob.png", &GOB_TEXTURE, GL_RGB);
}

void load_texture(std::string path, u32* texture_id, int color_format) {
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
