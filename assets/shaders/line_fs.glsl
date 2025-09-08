#version 330 core

in vec3 _color;

out vec4 FragColor;

void main() {
    // FragColor = vec4(_color, 1);
    FragColor = vec4(1, 0, 0, 1);
}
