#pragma once

#include "pch.hpp" // IWYU pragma: keep

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

#define DELTA_TIME STATS.delta_time

struct Stats {
    float delta_time = 0.166f;

  private:
    steady_clock::time_point clock = steady_clock::now();

    uint32_t NOW = SDL_GetPerformanceCounter();
    uint32_t LAST = 0;

    uint8_t COUNTER = 0;
    std::array<float, 32> FPSS = {};
    std::array<float, 32> UPDATE_TIMES = {};
    std::array<float, 32> RENDER_TIMES = {};

  public:
    Stats() {
        FPSS.fill(1.0f / 60.0f);
        UPDATE_TIMES.fill(100.0f);
    }

    void update(float update_time);
    void add_render_time(float time);
    float FPS();
    std::string UPDATE_TIME();
    std::string RENDER_TIME();
    float MS();
    float ELAPSED();
    long get_peak_mem_usage();
    long get_current_mem_usage();
};

extern Stats STATS;
