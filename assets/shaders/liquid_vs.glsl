#version 330 core

layout (location = 0) in vec3  pos;
layout (location = 1) in vec3  normal;
layout (location = 2) in vec2  uv;
layout (location = 3) in vec2  wpos;
layout (location = 4) in float color;

uniform mat4 view_projection;
uniform vec2 scroll;

out vec2  _uv;
out float _color;

void main() {
    vec3 final_pos = pos + vec3(wpos.x, 0, wpos.y);
    _uv            = uv + scroll;
    _color         = color;
    gl_Position    = view_projection * vec4(final_pos, 1);
}
