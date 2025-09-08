#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "cube.hpp"
#include "neighbors.hpp"

#define LATTICE_VERTS 8

struct Lattice {
    vec3 bb_min;
    vec3 bb_max;

    std::array<vec3, LATTICE_VERTS> vertices;

    void init_vertices();
    void from_floats(std::vector<float> &vs, bool cube);
    void from_vertices(std::vector<Vertex> &vs, bool cube);
    void from_world_vertices(std::array<WorldVertex, N_VERTS_PER_CUBE> &vs, bool cube);
    void add_slope_perlin(vec3 pos, float y_offset, float y_scale);
    void add_distortion_perlin(vec3 pos);
    void apply_to_vertex(vec3 vert);
    void add_offset(float amount);
    void match_liquid_level(Neighbors nbs, float amount);
    void apply_to_cube(Cube &c);
    void deform_vertex(vec3 base_index, vec3 weights, vec3 out);

    size ulb();
    size urb();
    size drb();
    size dlb();
    size ulf();
    size urf();
    size dlf();
    size drf();
};
