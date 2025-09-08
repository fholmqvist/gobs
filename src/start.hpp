#pragma once

#include "base.hpp"
#include "external/stb_image.h"

#include <SDL3/SDL.h>
#include <string>

int start();
bool init_SDL();
void load_assets();
void load_texture(std::string path, u32* texture_id, int color_format);
