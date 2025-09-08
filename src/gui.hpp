#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "text.hpp"

#define SC SELECTED_CHUNK
#define SB SELECTED_BLOCK
#define WP CURR_WORLD_POS

struct GUI {
  private:
    TextRenderer text_renderer;

    std::string fraps;
    std::string updat;
    std::string redat;
    std::string pmems;
    std::string cmems;

  public:
    void init();
    void update();
    void render();
};
