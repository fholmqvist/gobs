#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "base.hpp"
#include "systems.hpp"

struct Level {
    usize width;
    usize total;

    std::function<void(Level &l)> initf;

    Level(int wsize, std::function<void(Level &)> _initf)
        : width(wsize), total(wsize * wsize), initf(_initf) {};

    Systems systems;

    // TODO:
    usize n_indices;

    void init();
    void update();
    void render();

    GID add_liquid(LIQUID, ivec4);
    GID add_light(Light);
    GID add_model(MESH type, vec3 pos, vec3 rot, float scale);
    void play_animation(GID, ANIMATION);
};
