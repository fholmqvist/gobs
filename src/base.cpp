#include "base.hpp"

#include <cstdarg>
#include <numeric>
#include <string>

std::string frmt(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    return std::string(buffer);
}

std::pair<int, int> aspect_ratio(int width, int height) {
    int gcd = std::gcd(width, height);
    return { width / gcd, height / gcd };
}
