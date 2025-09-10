#include "models.hpp"
#include "sample_grid.hpp"

void Models::update() {
    for (auto &m : ms.values) {
        m.animator.update();
    }
}

GID Models::add(std::vector<WorldVertex> &grid, MESH type, vec3 pos, vec3 rot, float scale,
                 int wsize) {
    Model model(type, pos, rot, scale);

    vec3 normal;
    float avg_height = sample_floor_y(grid, pos[0], pos[2], wsize, normal);
    float floor_y = avg_height;
    model.move(vec2(0), floor_y, normal);

    return ms.add(model);
}
