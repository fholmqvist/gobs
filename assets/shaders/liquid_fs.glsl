#version 330 core

#include "helpers.glsl"

in vec2  _uv;
in float _color;

uniform sampler2D _texture;
uniform float     _transparency = 0.24;

out vec4 FragColor;

// TODO: Affected by lights!
void main() {
    vec3 shade = _color * vec3(0.5);
    vec4 color = texture(_texture, _uv) * vec4(shade, 1.0);
    FragColor = vec4(posterize(color, 32.0).rgb, _transparency);
}
