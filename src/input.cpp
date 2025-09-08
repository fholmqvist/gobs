#include "input.hpp"
#include "camera.hpp"

bool Input::update(int wsize) {
    bool running = true;

    const bool* keystate = SDL_GetKeyboardState(NULL);
    const auto mousestate = SDL_GetMouseState(NULL, NULL);

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
                    case SDLK_SPACE:
                        CAMERA->print_debug_information();
                        break;
                    case SDLK_M:
                        CAMERA->reset_view();
                        break;
                }
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                if (abs(e.wheel.y) > 0) {
                    CAMERA->mouse_scroll(e.wheel.y);
                }
                break;
            case SDL_EVENT_MOUSE_MOTION:
                CAMERA->mouse_pos.x = e.motion.x;
                CAMERA->mouse_pos.y = e.motion.y;

                if (mousestate & SDL_BUTTON_MMASK) {
                    int dx = e.motion.xrel;
                    int dy = e.motion.yrel;
                    CAMERA->mouse_movement((float)dx, (float)-dy);
                }
                break;
        }
    }

    const bool UP = keystate[SDL_SCANCODE_I];
    const bool DOWN = keystate[SDL_SCANCODE_K];
    const bool LEFT = keystate[SDL_SCANCODE_J];
    const bool RIGHT = keystate[SDL_SCANCODE_L];

    if (UP || DOWN || LEFT || RIGHT) {
        if (UP) {
            CAMERA->movement(vec3(0.0f, 0.0f, -1.0f));
        } else if (DOWN) {
            CAMERA->movement(vec3(0.0f, 0.0f, 1.0f));
        }
        if (LEFT) {
            CAMERA->movement(vec3(-1.0f, 0.0f, 0.0f));
        } else if (RIGHT) {
            CAMERA->movement(vec3(1.0f, 0.0f, 0.0f));
        }
    }

    CAMERA->update(*this, wsize);

    return running;
}
