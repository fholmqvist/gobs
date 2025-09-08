#include "timer.hpp"

#include "base.hpp"

using std::chrono::duration_cast;
using std::chrono::microseconds;
using std::chrono::steady_clock;

void Timer::reset() {
    clock = steady_clock::now();
}

uint64_t Timer::stop() {
    uint64_t dur = duration_cast<microseconds>(steady_clock::now() - clock).count();
    reset();
    return dur;
}

std::string Timer::stop_string() {
    auto dur = stop();
    return time_to_string(dur);
}

Timer GLOBAL_TIMER;
