#version 330 core

in vec2  _uv;
in float _color;

uniform sampler2D _texture;
uniform float     _transparency = 0.24;

out vec4 FragColor;

vec4 posterize(vec4 color);

// TODO: Affected by lights!
void main() {
    vec3 shade = _color * vec3(0.5);
    vec4 color = texture(_texture, _uv) * vec4(shade, 1.0);
    FragColor = vec4(posterize(color).rgb, _transparency);
}

vec4 posterize(vec4 color) {
    float gamma = 0.6;
    float num_colors = 32.0;

    vec3 color_out = color.rgb;
    color_out = pow(color_out, vec3(gamma));
    color_out = color_out * num_colors;
    color_out = floor(color_out);
    color_out = color_out / num_colors;
    color_out = pow(color_out, vec3(1.0 / gamma));

    return vec4(color_out, 1.0);
}

