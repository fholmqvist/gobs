#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "level.hpp"

struct Shader {
    u32 id;
    u32 vao;
    u32 vbo;

    std::string vertex_source;
    std::string fragment_source;

    std::optional<std::function<void(Shader &)>> init_func;
    std::function<void(Shader &, Level &)> draw_func;

    Shader(std::string vertex_source, std::string fragment_source,
           std::function<void(Shader &, Level &)> draw_func)
        : vertex_source(vertex_source), fragment_source(fragment_source), draw_func(draw_func) {
    }

    Shader(std::string vertex_source, std::string fragment_source,
           std::function<void(Shader &)> init_func,
           std::function<void(Shader &, Level &)> draw_func)
        : vertex_source(vertex_source), fragment_source(fragment_source), init_func(init_func),
          draw_func(draw_func) {
    }

    void init();

    void render(Level &l);
};

void store_glsl_helper(std::string source);

void compile_shaders(u32 shader_id, std::string vertex_source, std::string fragment_source);
