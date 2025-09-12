#include "lights.hpp"

GID Lights::add(Light l) {
    return lights.add(l);
}

LightUBO Lights::pack(std::vector<Light> ls) {
    LightUBO out;
    out.header[0] = (float)ls.size();

    // pos(vec4):    xyz = position, w = 0
    // color(vec4):  rgb = color (linear), a = intensity
    // params(vec4): x   = radius, yzw unused
    for (size i = 0; i < (size)ls.size(); ++i) {
        size base = i * 12; // 3 vec4s
        // pos
        out.data[base + 0] = ls[i].pos[0];
        out.data[base + 1] = ls[i].pos[1];
        out.data[base + 2] = ls[i].pos[2];
        out.data[base + 3] = 0.0f;
        // color and intensity
        out.data[base + 4] = ls[i].color[0];
        out.data[base + 5] = ls[i].color[1];
        out.data[base + 6] = ls[i].color[2];
        out.data[base + 7] = ls[i].intensity;
        // params (radius)
        out.data[base + 8] = ls[i].radius;
        out.data[base + 9] = 0.0f;
        out.data[base + 10] = 0.0f;
        out.data[base + 11] = 0.0f;
    }

    return out;
}

std::vector<Light> Lights::build(SphereBound bounds) {
    std::vector<Light> out;

    for (auto light : lights.values) {
        SphereBound other = SphereBound::from_point_light(light);
        if (light.intensity < 0.001f || !bounds.intersect(other)) {
            continue;
        }
        out.push_back(light);
    }

    std::sort(out.begin(), out.end(), [&](const Light &a, const Light &b) {
        return distance(a.pos, bounds.pos) < distance(b.pos, bounds.pos);
    });

    return out;
}
