#version 330 core

#include "helpers.glsl"

in vec2  _uv;
in float _vcolor;
in vec3  _light;

uniform sampler2D _texture;

out vec4 FragColor;

void main() {
    vec4 color = model_color(_vcolor, _light, _texture, _uv);
    FragColor = posterize(color, 64.0);
}
