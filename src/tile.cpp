#include "tile.hpp"

#include "base.hpp"

TileUV tile_get_uvs(TILE t) {
    switch (t) {
        case TILE::ROCK_GROUND:
            return get_tile_uv(1, 0);
        case TILE::ROCK:
            return get_tile_uv(0, 0);
        case TILE::BRICK_GROUND:
        case TILE::BRICK:
            return get_tile_uv(2, 0);
        case TILE::WATER_GROUND:
            return get_tile_uv(3, 0);
        default:
            log_dang("Unrecognized tile %d", t);
            return (TileUV){};
    }
}

TileUV tile_get_matching_uvs(TILE t) {
    switch (t) {
        case TILE::ROCK_GROUND:
            t = TILE::ROCK;
            break;
        case TILE::BRICK_GROUND:
            t = TILE::BRICK;
            break;
        default:
            break;
    }
    return tile_get_uvs(t);
}
