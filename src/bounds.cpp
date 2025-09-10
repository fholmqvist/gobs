#include "bounds.hpp"

SphereBound SphereBound::from_point_light(Light p) {
    SphereBound out;
    out.pos = p.pos;
    out.radius = p.radius;
    return out;
}

bool SphereBound::intersect(SphereBound other) {
    vec3 d = pos - other.pos;
    float r = radius + other.radius;
    return dot(d, d) < (r * r);
}
