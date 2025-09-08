#version 330 core

in vec2  _uv;
in float _vcolor;
in vec3  _light;

uniform sampler2D _texture;

out vec4 FragColor;

vec4 posterize(vec4 color);

void main() {
    FragColor = vec4(1, 0, 0, 1);

    // vec3 vcolor = vec3(_vcolor * 1.0, _vcolor * 0.9, _vcolor * 0.8);
    // vec3 ambient = vec3(1.0, 0.7, 0.5) * 0.08;
    // vec3 texture_color = texture(_texture, _uv).rgb;

    // // NOTE: Ensure that light_color never exceeds 1.0.
    // vec3 light_color = (_light * 0.9) + ambient;

    // vec3 final = texture_color * light_color * vcolor;

    // FragColor = posterize(vec4(final, 1));
}

vec4 posterize(vec4 color) {
    float gamma = 0.6;
    float num_colors = 64.0;

    vec3 color_out = color.rgb;
    color_out = pow(color_out, vec3(gamma));
    color_out = color_out * num_colors;
    color_out = floor(color_out);
    color_out = color_out / num_colors;
    color_out = pow(color_out, vec3(1.0 / gamma));

    return vec4(color_out, 1.0);
}
