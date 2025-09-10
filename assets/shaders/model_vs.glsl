#version 330 core

#include "helpers.glsl"

layout (location = 0) in vec3  pos;
layout (location = 1) in vec3  normal;
layout (location = 2) in vec2  uv;
layout (location = 3) in float vcolor;
layout (location = 4) in ivec4 bone_indexes;
layout (location = 5) in vec4  bone_weights;

uniform mat4 view_projection;
uniform vec3 world_pos;
uniform vec3 rotation = vec3(0.0);
uniform float scale = 1.0;

const int MAX_BONES = 64;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 final_bone_matrices[MAX_BONES];

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
    vec4 bone_pos    = vec4(0.0);
    vec3 bone_normal = vec3(0.0);
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++) {
        int index = bone_indexes[i];

        if (index == -1) {
            continue;
        }
        if (index >= MAX_BONES) {
            bone_pos = vec4(pos, 1.0);
            break;
        }

        vec4 local_pos = final_bone_matrices[index] * vec4(pos, 1.0);
        bone_pos += local_pos * bone_weights[i];
        // vec3 local_normal = mat3(final_bone_matrices[i]) * normal;
        // bone_normal += local_normal * bone_weights[i];
    }

    mat4 model = model_matrix(world_pos, rotation, scale);
    vec4 final_pos = model * bone_pos;

    // TODO:
    // vec3 final_normal = normal * bone_normal;
    vec3 final_normal = bone_pos.xyz * normal;
    // vec3 final_normal = normal;

    vec3 light = vec3(0.4);

    int count = int(header.x);
    for (int i = 0; i < count; i++) {
        vec3 l = lights[i].pos.xyz - final_pos.xyz;
        float d = length(l);

        if (d <= lights[i].params.x) {
            vec3 ldir = l / max(d, 1e-4);
            float ndotl = max(dot(normalize(final_normal), ldir), 0.0);
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
