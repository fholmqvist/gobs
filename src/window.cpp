#include "window.hpp"

#include "constants.hpp"
#include "timer.hpp"

bool Window::init() {
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

    glClearColor(0, 0, 0, 1);

    // VSYNC.
    SDL_GL_SetSwapInterval(1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    log_info("Initialized window in %s", time_to_string(GLOBAL_TIMER.stop()).c_str());

    return true;
}
