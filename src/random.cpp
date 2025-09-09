#include "random.hpp"

#define DB_PERLIN_IMPL
#include "external/db_perlin.hpp"

const float PERLIN_DEFAULT_STRENGTH = 1.0f;
const float PERLIN_U = 12.0f;
const float PERLIN_D = 18.0f;

double perlin2(double x, double y) {
    return db::perlin(x, y);
}

u64 Random::seed_value() {
    return seed;
}

int Random::integer() {
    idx++;
    if (idx == MAX_RAND) {
        idx = 0;
    }
    return rands[idx];
}

int Random::range(int min, int max) {
    return min + integer() % (max - min + 1);
}
