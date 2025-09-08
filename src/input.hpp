#pragma once

#include <SDL3/SDL_events.h>

struct Input {
    SDL_Event e;

    bool update();
};
