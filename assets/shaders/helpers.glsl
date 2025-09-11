vec4 model_color(float _vcolor, vec3 _light, sampler2D _texture, vec2 _uv) {
    vec3 vcolor = vec3(_vcolor * 1.0, _vcolor * 0.9, _vcolor * 0.8);
    vec3 ambient = vec3(1.0, 0.7, 0.5) * 0.4; //0.08;
    vec3 texture_color = texture(_texture, _uv).rgb;

    // NOTE: Ensure that light_color never exceeds 1.0.
    vec3 light_color = (_light * 0.9) + ambient;

    return vec4(texture_color * light_color * vcolor, 1);
}

vec4 posterize(vec4 color, float num_colors) {
    float gamma = 0.6;

    vec3 color_out = color.rgb;
    color_out = pow(color_out, vec3(gamma));
    color_out = color_out * num_colors;
    color_out = floor(color_out);
    color_out = color_out / num_colors;
    color_out = pow(color_out, vec3(1.0 / gamma));

    return vec4(color_out, 1.0);
}

vec4 to_low_precision(vec4 position) {
    vec2 resolution = vec2(640, 480);
    vec3 perspective_divide = position.xyz / vec3(position.w);
    vec2 screen_coords = (perspective_divide.xy + vec2(1.0,1.0)) * vec2(resolution.x,resolution.y) * 0.5;
    vec2 screen_coords_truncated = vec2(int(screen_coords.x), int(screen_coords.y));
    vec2 reconverted_xy = ((screen_coords_truncated * vec2(2,2)) / vec2(resolution.x,resolution.y)) - vec2(1,1);
    vec4 ps1_pos = vec4(reconverted_xy.x, reconverted_xy.y, perspective_divide.z, position.w);
    ps1_pos.xyz = ps1_pos.xyz * vec3(position.w, position.w, position.w);
    return ps1_pos;
}

float attenuation(float dist, float radius) {
    float x = clamp(1.0 - dist / max(radius, 1e-4), 0.0, 1.0);
    return x * x;
}


mat4 translate(mat4 m, vec3 v) {
    m[3] = m[3] + vec4(v, 0.0);
    return m;
}

mat4 mscale(mat4 m, vec3 v) {
    m[0] *= v.x;
    m[1] *= v.y;
    m[2] *= v.z;
    return m;
}

mat4 rotate(mat4 m, float angle, vec3 axis) {
    float c = cos(angle);
    float s = sin(angle);
    vec3 a = normalize(axis);
    vec3 temp = (1.0 - c) * a;

    mat4 rot = mat4(1.0);
    rot[0][0] = c + temp.x * a.x;
    rot[0][1] = temp.x * a.y + s * a.z;
    rot[0][2] = temp.x * a.z - s * a.y;

    rot[1][0] = temp.y * a.x - s * a.z;
    rot[1][1] = c + temp.y * a.y;
    rot[1][2] = temp.y * a.z + s * a.x;

    rot[2][0] = temp.z * a.x + s * a.y;
    rot[2][1] = temp.z * a.y - s * a.x;
    rot[2][2] = c + temp.z * a.z;

    return m * rot;
}

mat4 model_matrix(vec3 position, vec3 rotation, float scale) {
    mat4 m = mat4(1.0);
    m = translate(m, position);
    m = rotate(m, rotation.y, vec3(0.0, 1.0, 0.0));
    m = rotate(m, rotation.x, vec3(1.0, 0.0, 0.0));
    m = rotate(m, rotation.z, vec3(0.0, 0.0, 1.0));
    m = mscale(m, vec3(scale));
    return m;
}
