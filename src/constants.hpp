#pragma once

#include "base.hpp"

extern const int SCREEN_W;
extern const int SCREEN_H;

extern const u8 CSIZE;

extern const float SHADOW_STRENGTH;
extern const float WATER_LEVEL;

#define XY_TO_IDX(x, y) ((usize)(x % WSIZE) + (usize)(y * WSIZE))
#define TO_GRID(x) ((int)(floorf(x) / CSIZE))

extern const float SHADOW_STRENGTH;
extern const float WATER_LEVEL;

extern u32 FONT_TEXTURE;

extern u32 TILE_MAP_TEXTURE;
extern u32 WATER_TEXTURE;
extern u32 RAIL_TEXTURE;
extern u32 RAIL_CART_TEXTURE;
extern u32 PIPE_TEXTURE;

extern u32 GOB_TEXTURE;
