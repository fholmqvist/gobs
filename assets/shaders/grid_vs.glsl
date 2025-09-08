#version 330 core

layout (location = 0) in vec3  pos;
layout (location = 1) in vec3  normal;
layout (location = 2) in vec2  uv;
layout (location = 3) in vec2  wpos;
layout (location = 4) in float vcolor;

uniform mat4 view_projection;

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

void main() {
    vec3 final_pos = pos + vec3(wpos.x, 0, wpos.y);

    vec3 light = vec3(0.0);

    int count = int(header.x);
    for (int i = 0; i < count; i++) {
        vec3 l = lights[i].pos.xyz - final_pos;
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
    // gl_Position = to_low_precision(view_projection * vec4(final_pos, 1));
    gl_Position = view_projection * vec4(final_pos, 1);
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
