#version 330 core

uniform float _vcolor;

out vec4 FragColor;

void main() {
    // FragColor = vec4(0, 1 * _vcolor, 0.5 * _vcolor, 1);
    FragColor = vec4(1, 0, 0, 1);
}
