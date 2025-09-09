#include "lattice.hpp"

#include "cube.hpp"
#include "neighbors.hpp"
#include "random.hpp"

const int LATTICE_SIZE = 2;
const float LS = (float)LATTICE_SIZE - 1;

size get_index(size i, size j, size k);
float float_mix(float a, float b, float t);

void Lattice::from_floats(std::vector<float> &vs, bool cube) {
    if (cube) {
        bb_min = vec3(-0.5f, -0.5f, -0.5f);
        bb_max = vec3(0.5f, 0.5f, 0.5f);
    } else {
        bb_min = { FLT_MAX, FLT_MAX, FLT_MAX };
        bb_max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

        for (size i = 0; i < (size)vs.size(); i += VERT_STRIDE) {
            vec3 pos = {
                vs[i + 0],
                vs[i + 1],
                vs[i + 2],
            };
            bb_min = glm::min(pos, bb_min);
            bb_max = glm::min(pos, bb_max);
        }
    }

    init_vertices();
}

void Lattice::from_vertices(std::vector<Vertex> &vs, bool cube) {
    if (cube) {
        bb_min = vec3(-0.5f, -0.5f, -0.5f);
        bb_max = vec3(0.5f, 0.5f, 0.5f);
    } else {
        bb_min = { FLT_MAX, FLT_MAX, FLT_MAX };
        bb_max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

        for (size i = 0; i < (size)vs.size(); i++) {
            vec3 pos = vs[i].pos;
            bb_min = glm::min(pos, bb_min);
            bb_max = glm::max(pos, bb_max);
        }
    }

    init_vertices();
}

void Lattice::from_world_vertices(std::array<WorldVertex, N_VERTS_PER_CUBE> &vs, bool cube) {
    if (cube) {
        bb_min = vec3(-0.5f, -0.5f, -0.5f);
        bb_max = vec3(0.5f, 0.5f, 0.5f);
    } else {
        bb_min = { FLT_MAX, FLT_MAX, FLT_MAX };
        bb_max = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

        for (size i = 0; i < N_VERTS_PER_CUBE; i++) {
            vec3 pos = vs[i].pos;
            bb_min = glm::min(pos, bb_min);
            bb_max = glm::max(pos, bb_max);
        }
    }

    init_vertices();
}

void Lattice::add_slope_perlin(vec3 pos, float y_offset, float y_scale) {
    Perlins upper_verts(pos, PERLIN_U);
    Perlins lower_verts(pos, PERLIN_D);

    vertices[ulb()][1] += (upper_verts.tl * y_scale) + y_offset;
    vertices[urb()][1] += (upper_verts.tr * y_scale) + y_offset;
    vertices[urf()][1] += (upper_verts.br * y_scale) + y_offset;
    vertices[ulf()][1] += (upper_verts.bl * y_scale) + y_offset;

    vertices[dlb()][1] += (lower_verts.tl * y_scale) + y_offset;
    vertices[drb()][1] += (lower_verts.tr * y_scale) + y_offset;
    vertices[drf()][1] += (lower_verts.br * y_scale) + y_offset;
    vertices[dlf()][1] += (lower_verts.bl * y_scale) + y_offset;
}

void Lattice::add_distortion_perlin(vec3 pos) {
    const float amount = 0.55f;
    const float strength = 0.032f;

    Perlins upper_verts(pos, amount);
    Perlins lower_verts(pos, amount);

    vertices[ulb()][0] += upper_verts.tl * strength;
    vertices[urb()][0] += upper_verts.tr * strength;
    vertices[urf()][0] += upper_verts.br * strength;
    vertices[ulf()][0] += upper_verts.bl * strength;
    vertices[dlb()][0] += lower_verts.tl * strength;
    vertices[drb()][0] += lower_verts.tr * strength;
    vertices[drf()][0] += lower_verts.br * strength;
    vertices[dlf()][0] += lower_verts.bl * strength;

    vertices[ulb()][1] += upper_verts.tl * strength;
    vertices[urb()][1] += upper_verts.tr * strength;
    vertices[urf()][1] += upper_verts.br * strength;
    vertices[ulf()][1] += upper_verts.bl * strength;
    vertices[dlb()][1] += lower_verts.tl * strength;
    vertices[drb()][1] += lower_verts.tr * strength;
    vertices[drf()][1] += lower_verts.br * strength;
    vertices[dlf()][1] += lower_verts.bl * strength;

    vertices[ulb()][2] += upper_verts.tl * strength;
    vertices[urb()][2] += upper_verts.tr * strength;
    vertices[urf()][2] += upper_verts.br * strength;
    vertices[ulf()][2] += upper_verts.bl * strength;
    vertices[dlb()][2] += lower_verts.tl * strength;
    vertices[drb()][2] += lower_verts.tr * strength;
    vertices[drf()][2] += lower_verts.br * strength;
    vertices[dlf()][2] += lower_verts.bl * strength;
}

void Lattice::add_offset(float amount) {
    vertices[ulb()][1] += amount;
    vertices[urb()][1] += amount;
    vertices[urf()][1] += amount;
    vertices[ulf()][1] += amount;

    vertices[dlb()][1] += amount;
    vertices[drb()][1] += amount;
    vertices[drf()][1] += amount;
    vertices[dlf()][1] += amount;
}

void Lattice::match_liquid_level(Neighbors nbs, float amount) {
    if (nbs.up()) {
        vertices[dlb()][1] += amount;
        vertices[drb()][1] += amount;
    } else if (nbs.right()) {
        vertices[drb()][1] += amount;
        vertices[drf()][1] += amount;
    } else if (nbs.down()) {
        vertices[dlf()][1] += amount;
        vertices[drf()][1] += amount;
    } else if (nbs.left()) {
        vertices[dlb()][1] += amount;
        vertices[dlf()][1] += amount;
    } else if (nbs.down_right()) {
        vertices[drf()][1] += amount;
    } else if (nbs.up_right()) {
        vertices[drb()][1] += amount;
    } else if (nbs.down_left()) {
        vertices[dlf()][1] += amount;
    } else if (nbs.left()) {
        vertices[dlb()][1] += amount;
        vertices[dlf()][1] += amount;
    } else if (nbs.up_left()) {
        vertices[dlb()][1] += amount;
    }
}

vec3 Lattice::apply_to_vertex(vec3 vert) {
    vec3 local = (vert - bb_min) / (bb_max - bb_min);
    glm::clamp(local, 0.0f, 0.999f);

    vec3 grid = local * vec3{ LS, LS, LS };

    vec3 base = glm::floor(grid);

    vec3 weights = grid - base;

    return Lattice::deform_vertex(base, weights);
}

void Lattice::apply_to_cube(Cube &c) {
    for (size i = 0; i < (size)c.verts.size(); i++) {
        vec3 vert = c.verts[i].pos;
        vert = Lattice::apply_to_vertex(vert);
        c.verts[i].pos[1] = vert[1];
    }
}

void Lattice::init_vertices() {
    u8 initialized = 0;

    for (size k = 0; k < LATTICE_SIZE; k++) {
        for (size j = 0; j < LATTICE_SIZE; j++) {
            for (size i = 0; i < LATTICE_SIZE; i++) {
                size idx = get_index(i, j, k);
                vertices[idx] = { float_mix(bb_min[0], bb_max[0], (float)i / LS),
                                  float_mix(bb_min[1], bb_max[1], (float)j / LS),
                                  float_mix(bb_min[2], bb_max[2], (float)k / LS) };
                initialized++;
            }
        }
    }

    assert(initialized == LATTICE_VERTS);
}

vec3 Lattice::deform_vertex(vec3 base_index, vec3 weights) {
    vec3 out = vec3(0);

    for (int dz = 0; dz <= 1; ++dz) {
        for (int dy = 0; dy <= 1; ++dy) {
            for (int dx = 0; dx <= 1; ++dx) {
                size i = (size)base_index[0] + dx;
                size j = (size)base_index[1] + dy;
                size k = (size)base_index[2] + dz;

                i = clamp(i, 0, LS);
                j = clamp(j, 0, LS);
                k = clamp(k, 0, LS);

                vec3 pos = vertices[get_index(i, j, k)];

                float wx = dx ? weights[0] : (1.0f - weights[0]);
                float wy = dy ? weights[1] : (1.0f - weights[1]);
                float wz = dz ? weights[2] : (1.0f - weights[2]);
                float w = wx * wy * wz;

                out += pos * w;
            }
        }
    }

    return out;
}

size get_index(size i, size j, size k) {
    assert(i >= 0 || j >= 0 || k >= 0 || i < LATTICE_SIZE || j < LATTICE_SIZE || k < LATTICE_SIZE);
    return i + j * LATTICE_SIZE + k * LATTICE_SIZE * LATTICE_SIZE;
}

float float_mix(float a, float b, float t) {
    return a * (1.0f - t) + b * t;
}

size Lattice::ulb() {
    return get_index(0, 1, 0);
}

size Lattice::urb() {
    return get_index(1, 1, 0);
}

size Lattice::drb() {
    return get_index(1, 0, 0);
}

size Lattice::dlb() {
    return get_index(0, 0, 0);
}

size Lattice::ulf() {
    return get_index(0, 1, 1);
}

size Lattice::urf() {
    return get_index(1, 1, 1);
}

size Lattice::dlf() {
    return get_index(0, 0, 1);
}

size Lattice::drf() {
    return get_index(1, 0, 1);
}
