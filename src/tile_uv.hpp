#pragma once

struct TileUV {
    float x1, x2;
    float y1, y2;

};

TileUV get_tile_uv(int tile_x, int tile_y);

extern const int ATLAS_WIDTH;
extern const int ATLAS_HEIGHT;

extern const float TILE_UV_X;
extern const float TILE_UV_Y;
