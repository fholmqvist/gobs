#version 330 core

in vec2 _uv;
in vec3 _color;

uniform sampler2D _texture;

out vec4 FragColor;

void main() {
    FragColor = texture(_texture, _uv) * vec4(_color, 1);
}
