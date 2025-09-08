#version 330 core

layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 uv;

out vec2 _uv;
out vec3 _color;

uniform mat4 mvp;
uniform vec3 color;

void main() {
    gl_Position = mvp * vec4(pos, 0, 1);
    _uv = uv;
    _color = color;
}
