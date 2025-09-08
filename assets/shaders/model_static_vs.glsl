#version 330 core

layout (location = 0) in vec3  pos;
layout (location = 1) in vec3  normal;
layout (location = 2) in vec2  uv;
layout (location = 3) in float vcolor;

uniform mat4 view_projection;
uniform vec3 world_pos;
uniform vec3 rotation = vec3(0.0);
uniform float scale = 1.0;

out vec2  _uv;
out float _vcolor;
out vec3  _light;

// Each light:
//   pos    : xyz (world space), w unused
//   color  : rgb (linear), a = intensity scalar
//   params : x = radius (cutoff distance), y,z,w unused
struct Light {
    vec4 pos;
    vec4 color;
    vec4 params;
};

layout(std140) uniform Lights {
    vec4 header; // x = count
    Light lights[64];
};

vec4 to_low_precision(vec4 position);
float attenuation(float dist, float radius);
mat4 model_matrix(vec3 position, vec3 rotation, float scale);

void main() {
    mat4 model = model_matrix(world_pos, rotation, scale);
    vec4 final_pos = model * vec4(pos, 1.0);

    vec3 light = vec3(0.0);

    int count = int(header.x);
    for (int i = 0; i < count; i++) {
        vec3 l = lights[i].pos.xyz - final_pos.xyz;
        float d = length(l);

        if (d <= lights[i].params.x) {
            vec3 ldir = l / max(d, 1e-4);
            float ndotl = max(dot(normalize(normal), ldir), 0.0);
            float att = attenuation(d, lights[i].params.x);
            float intensity = lights[i].color.a;  // scalar
            light += lights[i].color.rgb * (ndotl * att * intensity);
        }
    }

    _uv         = uv;
    _vcolor     = vcolor;
    _light      = light;
    gl_Position = to_low_precision(view_projection * final_pos);
}

vec4 to_low_precision(vec4 position) {
    vec2 resolution = vec2(640, 480);
    vec3 perspective_divide = position.xyz / vec3(position.w);
    vec2 screen_coords = (perspective_divide.xy + vec2(1.0,1.0)) * vec2(resolution.x,resolution.y) * 0.5;
    vec2 screen_coords_truncated = vec2(int(screen_coords.x), int(screen_coords.y));
    vec2 reconverted_xy = ((screen_coords_truncated * vec2(2,2)) / vec2(resolution.x,resolution.y)) - vec2(1,1);
    vec4 ps1_pos = vec4(reconverted_xy.x, reconverted_xy.y, perspective_divide.z, position.w);
    ps1_pos.xyz = ps1_pos.xyz * vec3(position.w, position.w, position.w);
    return ps1_pos;
}

float attenuation(float dist, float radius) {
    float x = clamp(1.0 - dist / max(radius, 1e-4), 0.0, 1.0);
    return x * x;
}

mat4 translate(mat4 m, vec3 v) {
    m[3] = m[3] + vec4(v, 0.0);
    return m;
}

mat4 mscale(mat4 m, vec3 v) {
    m[0] *= v.x;
    m[1] *= v.y;
    m[2] *= v.z;
    return m;
}

mat4 rotate(mat4 m, float angle, vec3 axis) {
    float c = cos(angle);
    float s = sin(angle);
    vec3 a = normalize(axis);
    vec3 temp = (1.0 - c) * a;

    mat4 rot = mat4(1.0);
    rot[0][0] = c + temp.x * a.x;
    rot[0][1] = temp.x * a.y + s * a.z;
    rot[0][2] = temp.x * a.z - s * a.y;

    rot[1][0] = temp.y * a.x - s * a.z;
    rot[1][1] = c + temp.y * a.y;
    rot[1][2] = temp.y * a.z + s * a.x;

    rot[2][0] = temp.z * a.x + s * a.y;
    rot[2][1] = temp.z * a.y - s * a.x;
    rot[2][2] = c + temp.z * a.z;

    return m * rot;
}

mat4 model_matrix(vec3 position, vec3 rotation, float scale) {
    mat4 m = mat4(1.0);
    m = translate(m, position);
    m = rotate(m, rotation.y, vec3(0.0, 1.0, 0.0));
    m = rotate(m, rotation.x, vec3(1.0, 0.0, 0.0));
    m = rotate(m, rotation.z, vec3(0.0, 0.0, 1.0));
    m = mscale(m, vec3(scale));
    return m;
}
