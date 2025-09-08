#include "camera.hpp"

#include "constants.hpp"
#include "pch.hpp" // IWYU pragma: keep

#include "input.hpp"
#include "stats.hpp"

std::unique_ptr<Camera> CAMERA = std::make_unique<Camera>(vec3(0.0f, 0.0f, 3.0f));

const mat4 PERSPECTIVE = perspective(radians(CAMERA->FOV), 800.0f / 600.0f, 0.1f, 100.0f);
const mat4 ORTHO = ortho(0.0f, (float)SCREEN_W, (float)SCREEN_H, 0.0f, -1.0f, 1.0f);

const float DEFAULT_YAW = -90.0f;
const float DEFAULT_PITCH = -60.0f;

const float SPEED = 2.5f;
const float SENSITIVITY = 0.01f;

#define cs mouse_state.click_state

mat4 Camera::view_matrix() {
    return lookAt(pos, pos + front, up);
}

void Camera::update(Input &input, int wsize) {
    (void)input;

    pos += vel * STATS.delta_time;
    const float damping = powf(0.9f, STATS.delta_time * MOVE_SPEED);
    vel *= damping;

    pos.x = std::clamp(pos.x, -8.0f, (float)wsize + 8);
    pos.z = std::clamp(pos.z, -8.0f, (float)wsize + 8);

    // TODO: Smooth clamping by reducing
    //       vel.y when y is close to 0.
    // pos.y = std::clamp(pos.y, 1.5f, 8.0f);
    pos.y = std::clamp(pos.y, 1.0f, 8.0f);

    if (length(vel) < 0.001f) {
        vel = vec3(0.0f);
    }

    yaw += mouse_rot_vel.x;
    pitch += mouse_rot_vel.y;

    mouse_rot_vel *= damping;

    pitch = std::clamp(pitch, -88.0f, -10.0f);

    update_vectors();
}

void Camera::movement(vec3 move) {
    vec3 flat_front = normalize(vec3(front.x, 0.0f, front.z));
    vec3 flat_right = normalize(cross(flat_front, { 0, 1, 0 }));

    vec3 direction = move.x * flat_right + move.z * -flat_front;

    vel += direction * STATS.delta_time * MOVE_SPEED;
}

void Camera::mouse_movement(float x_offset, float y_offset) {
    x_offset *= mouse_sensitivity;
    y_offset *= mouse_sensitivity;

    mouse_rot_vel += vec2(x_offset, y_offset);
}

void Camera::mouse_scroll(int8_t dir) {
    if (dir == 1) {
        vel.y += 1;
    } else if (dir == -1) {
        vel.y -= 1;
    }
}

void Camera::reset_view() {
    yaw = DEFAULT_YAW;
    pitch = DEFAULT_PITCH;
    mouse_rot_vel = vec2(0);
    update_vectors();
}

void Camera::update_vectors() {
    vec3 new_front;
    new_front.x = cos(radians(yaw)) * cos(radians(pitch));
    new_front.y = sin(radians(pitch));
    new_front.z = sin(radians(yaw)) * cos(radians(pitch));
    front = normalize(new_front);
    right = normalize(cross(front, w_up));
    up = normalize(cross(right, front));
}

void Camera::print_debug_information() {
    log_info("Camera");
    printf("  pos: { %0.1f, %0.1f, %0.1f }\n", pos.x, pos.y, pos.z);
    printf("  rot: { %0.1f, %0.1f }\n", yaw, pitch);
}
