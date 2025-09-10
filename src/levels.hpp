#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "level.hpp"

typedef std::function<Level()> LevelF;

extern LevelF debug_level;
