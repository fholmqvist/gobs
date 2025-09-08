#include "ivec2.hpp"
#include "constants.hpp"

ivec2 ivec2_from_idx(size idx, usize wsize) {
    return ivec2{
        (int)idx % wsize,
        (int)idx / wsize,
    };
}

size ivec2_to_idx(ivec2 v, usize wsize) {
    return (usize)(v[0] + (v[1] * wsize));
}

void ivec2_add_csize(ivec2 v) {
    v[0] = TO_GRID((float)(v[0] + CSIZE));
    v[1] = TO_GRID((float)(v[1] + CSIZE));
}
