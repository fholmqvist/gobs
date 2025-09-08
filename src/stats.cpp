#include "stats.hpp"

#include "base.hpp"

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;

void Stats::update() {
    LAST = NOW;
    NOW = SDL_GetPerformanceCounter();
    delta_time = (float)((NOW - LAST) / (float)SDL_GetPerformanceFrequency());

    FPSS[COUNTER] = delta_time;
    if (++COUNTER >= FPSS.size()) {
        COUNTER = 0;
    }
}

void Stats::add_update_time(float time) {
    UPDATE_TIMES[COUNTER] = time;
}

void Stats::add_render_time(float time) {
    RENDER_TIMES[COUNTER] = time;
}

#define ACC(x) (std::accumulate(x.begin(), x.end(), 0.0f) / x.size())

float Stats::FPS() {
    float avg = ACC(FPSS);
    return 1 / avg;
}

std::string Stats::UPDATE_TIME() {
    float avg = ACC(UPDATE_TIMES);
    return time_to_string(avg);
}

std::string Stats::RENDER_TIME() {
    float avg = ACC(RENDER_TIMES);
    return time_to_string(avg);
}

float Stats::MS() {
    return 1000 * delta_time;
}

float Stats::ELAPSED() {
    return (duration_cast<milliseconds>(steady_clock::now() - clock).count()) / 1000.0;
}

long Stats::get_peak_mem_usage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss;
}

long Stats::get_current_mem_usage() {
    auto file = fopen("/proc/self/status", "r");
    char line[256];
    long result = 0;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "VmRSS:", 6) == 0) {
            sscanf(line + 6, "%ld", &result);
            break;
        }
    }

    fclose(file);
    return result;
}

Stats STATS;
