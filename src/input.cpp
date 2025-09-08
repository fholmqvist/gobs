#include "input.hpp"

bool Input::update() {
    bool running = true;

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_EVENT_QUIT:
                running = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                switch (e.key.key) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                }
        }
    }

    return running;
}
