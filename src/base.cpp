#include "base.hpp"

std::string cyan_string(const std::string &str) {
    return "\033[36m" + str + "\033[0m";
}

std::string red_string(const std::string &str) {
    return "\033[31m" + str + "\033[0m";
}

void log_info(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    printf("%s %s\n", cyan_string("[INFO]").c_str(), buffer);
}

void log_dang(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    printf("%s %s\n", red_string("[DANG]").c_str(), buffer);
    abort();
}

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

std::string time_to_string(u64 time, int precision) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision);

    if (time < 1000) {
        oss << time << "us";
    } else {
        oss << (time / 1000) << "ms";
    }

    return oss.str();
}
