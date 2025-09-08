#include "tile_uv.hpp"

const int ATLAS_WIDTH = 4;
const int ATLAS_HEIGHT = 4;

const float TILE_UV_X = 1.0f / (float)ATLAS_WIDTH;
const float TILE_UV_Y = 1.0f / (float)ATLAS_HEIGHT;

TileUV get_tile_uv(int tile_x, int tile_y) {
    float x_min = (float)tile_x * TILE_UV_X;
    float x_max = (float)x_min + TILE_UV_X;

    float y_min = (float)tile_y * TILE_UV_Y;
    float y_max = (float)y_min + TILE_UV_Y;

    return (TileUV){ x_min, x_max, y_min, y_max };
}
