#pragma once

#include "tile_uv.hpp"

enum struct TILE : u8 {
    ROCK_GROUND,
    ROCK,
    BRICK_GROUND,
    BRICK,
    WATER_GROUND,
};

TileUV tile_get_uvs(TILE t);
TileUV tile_get_matching_uvs(TILE t);
