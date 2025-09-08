#pragma once


#include "base.hpp"

#include <SDL3/SDL_render.h>

extern const int SCREEN_W;
extern const int SCREEN_H;

extern const u8 CSIZE;

#define WSIZE (LEVEL->wsize)
#define WTOTAL (LEVEL->wtotal)

#define XY_TO_IDX(x, y) ((usize)(x % WSIZE) + (usize)(y * WSIZE))
#define TO_GRID(x) ((int)(floorf(x) / CSIZE))

extern const float SHADOW_STRENGTH;
extern const float WATER_LEVEL;

extern float DELTA_TIME;

extern bool DISABLE_SHADERS;
extern SDL_Window* WINDOW;
extern SDL_GLContext GL_CONTEXT;
extern SDL_Event EVENT;

extern u32 FONT_TEXTURE;

extern u32 TILE_MAP_TEXTURE;
extern u32 WATER_TEXTURE;
extern u32 RAIL_TEXTURE;
extern u32 RAIL_CART_TEXTURE;
extern u32 PIPE_TEXTURE;

extern u32 SKAL_TEXTURE;
extern u32 GOB_TEXTURE;

#define SCENE_LIGHTS_MAX 512u
#define OBJECT_LIGHTS_MAX 8u
