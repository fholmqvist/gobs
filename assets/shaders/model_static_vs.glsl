#version 330 core

#include "helpers.glsl"

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
