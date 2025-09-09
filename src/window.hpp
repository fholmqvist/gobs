#pragma once

#include "pch.hpp" // IWYU pragma: keep

struct Window {
    bool DISABLE_SHADERS;
    SDL_Window* WINDOW;
    SDL_GLContext GL_CONTEXT;

    bool init();
};
