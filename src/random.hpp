#pragma once

#include "pch.hpp" // IWYU pragma: keep

extern const float PERLIN_DEFAULT_STRENGTH;
extern const float PERLIN_U;
extern const float PERLIN_D;

double perlin2(double x, double y);

#define MAX_RAND 255

struct Perlins {
    float tl;
    float tr;
    float bl;
    float br;

    Perlins(const vec3 &pos, float factor) {
        tl = perlin2(pos.x / factor, pos.z / factor);
        tr = perlin2((pos.x + 1) / factor, pos.z / factor);
        bl = perlin2(pos.x / factor, (pos.z + 1) / factor);
        br = perlin2((pos.x + 1) / factor, (pos.z + 1) / factor);
    }

    Perlins(const ivec3 &pos, float factor) {
        tl = perlin2(pos.x / factor, pos.z / factor);
        tr = perlin2((pos.x + 1) / factor, pos.z / factor);
        bl = perlin2(pos.x / factor, (pos.z + 1) / factor);
        br = perlin2((pos.x + 1) / factor, (pos.z + 1) / factor);
    }
};

struct Random {
  private:
    u64 seed;
    std::array<int, MAX_RAND> rands;
    u8 idx;

  public:
    Random(std::string input) {
        std::hash<std::string> hash_fn;
        size_t hash_value = hash_fn(input);
        seed = static_cast<u64>(hash_value);
        for (int i = 0; i < MAX_RAND; i++) {
            rands[i] = rand();
        }
    }

    u64 seed_value();
    int integer();
    int range(int min, int max);
};
