#pragma once

extern const float DEFAULT_YAW;
extern const float DEFAULT_PITCH;

using namespace glm;

extern const mat4 PERSPECTIVE;
extern const mat4 ORTHO;

struct Input;
struct MouseState;

extern const float DEFAULT_YAW;
extern const float DEFAULT_PITCH;

struct Camera {
    vec3 pos;
    vec3 vel = vec3(0);

    vec3 front;
    vec3 up;
    vec3 right;
    vec3 w_up = vec3(0, 1, 0);

    const float FOV = 45.0f;
    const float MOVE_SPEED = 60.0f;

    float yaw;
    float pitch;

    vec2 mpos;
    vec2 mrotv = vec2(0);
    float move_speed = 60.0f;
    float mouse_sensitivity = 0.5f;

    Camera(vec3 _pos = vec3(0), float _yaw = DEFAULT_YAW, float _pitch = DEFAULT_PITCH)
        : front(vec3(0, 0, -1)) {
        pos = _pos;
        yaw = _yaw;
        pitch = _pitch;
        update_vectors();
    }

    void update(Input &input, int wsize);
    void movement(vec3 move);
    void mouse_movement(float x_offset, float y_offset);
    void mouse_scroll(int8_t dir);
    void reset_view();
    void update_vectors();
    void print_debug_information();
    mat4 view_matrix();
};

extern std::unique_ptr<Camera> CAMERA;
