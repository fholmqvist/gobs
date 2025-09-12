#include "model.hpp"

#include "mesh.hpp"

Model::Model(Mesh _mesh, MESH _type, vec3 _pos, vec3 _rot, float _scale) {
    type = _type;
    mesh = _mesh;
    pos = _pos;
    _rot[1] = DEG2RAD(_rot[1]);
    rot = _rot;
    scale = _scale;
}

void Model::update() {
    animator.update();
}

static inline void pitch_roll_from_up_normal(vec3 n, float* out_pitch, float* out_roll) {
    vec3 u = normalize(vec3{ n[0], n[1], n[2] });

    float ny = u[1];
    float nz = u[2];
    float nx = u[0];

    float pitch = asinf(clamp(nz, -1.0f, 1.0f));
    float roll = atan2f(-nx, ny);

    *out_pitch = pitch;
    *out_roll = roll;
}

void Model::move(vec2 diff, float floor_y, vec3 floor_norm) {
    pos[0] += diff[0];
    pos[1] = floor_y;
    pos[2] += diff[1];

    float yaw = rot[1];

    float pitch_world, roll_world;
    pitch_roll_from_up_normal(floor_norm, &pitch_world, &roll_world);

    float cy = cosf(yaw);
    float sy = sinf(yaw);

    float pitch_local = cy * pitch_world - sy * roll_world;
    float roll_local = sy * pitch_world + cy * roll_world;

    rot[0] = pitch_local;
    rot[2] = roll_local;
}
