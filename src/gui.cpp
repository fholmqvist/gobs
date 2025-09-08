#include "gui.hpp"

#include "constants.hpp"
#include "stats.hpp"
#include "text.hpp"

#define SC SELECTED_CHUNK
#define SB SELECTED_BLOCK
#define WP CURR_WORLD_POS
#define SE SELECTED_ENTITY
#define DFS DEFAULT_FONT_SIZE

void GUI::init() {
    text_renderer.init(FONT_TEXTURE);
}

void GUI::update() {
}

void GUI::render() {
    fraps = frmt("FPS:             %.0f", STATS.FPS());
    updat = frmt("UPDATE TIME:     %s", STATS.UPDATE_TIME().c_str());
    redat = frmt("RENDER TIME:     %s (VSYNC)", STATS.RENDER_TIME().c_str());
    pmems = frmt("MEM CURRENT:     %ldMB", STATS.get_current_mem_usage() / 1024);
    cmems = frmt("MEM PEAK:        %ldMB", STATS.get_peak_mem_usage() / 1024);

    const float SPACE = 8.0f;
    const uint8_t SIZE = 8;

    text_renderer.render(fraps, SPACE, SPACE + (SIZE * 0), SIZE);
    text_renderer.render(updat, SPACE, SPACE + (SIZE * 1), SIZE);
    text_renderer.render(redat, SPACE, SPACE + (SIZE * 2), SIZE);
    text_renderer.render(pmems, SPACE, SPACE + (SIZE * 3), SIZE);
    text_renderer.render(cmems, SPACE, SPACE + (SIZE * 4), SIZE);
}
