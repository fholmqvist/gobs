#version 330 core

#include "helpers.glsl"

in vec2  _uv;
in float _vcolor;
in vec3  _light;

uniform sampler2D _texture;

out vec4 FragColor;

void main() {
    vec3 vcolor = vec3(_vcolor * 1.0, _vcolor * 0.9, _vcolor * 0.8);
    vec3 ambient = vec3(1.0, 0.7, 0.5) * 1.0; //0.08;
    vec3 texture_color = texture(_texture, _uv).rgb;

    // NOTE: Ensure that light_color never exceeds 1.0.
    vec3 light_color = (_light * 0.9) + ambient;

    vec3 final = texture_color * light_color * vcolor;

    FragColor = posterize(vec4(final, 1), 16.0);
}
