#include "storage.hpp"

template <typename T> //
GID Storage<T>::add(T value) {
    u32 s_idx;
    if (!free_indices.empty()) {
        s_idx = free_indices.back();
        free_indices.pop_back();
    } else {
        s_idx = static_cast<u32>(meta.size());
        meta.push_back({ 0, invalid_index });
    }

    u32 d_idx = static_cast<u32>(dense.size());
    dense.push_back(std::move(value));
    dense_to_sparse.push_back(s_idx);

    meta[s_idx].idx = d_idx;

    return GID{ s_idx, meta[s_idx].gen };
}

template <typename T> //
T &Storage<T>::get(GID id) {
    assert(valid(id));
    return dense[meta[id.idx].idx];
}

template <typename T> //
void Storage<T>::remove(GID id) {
    if (!valid(id)) {
        return;
    }

    u32 d_idx = meta[id.idx].idx;
    u32 last_d_idx = static_cast<u32>(dense.size() - 1);

    if (d_idx != last_d_idx) {
        std::swap(dense[d_idx], dense[last_d_idx]);
        u32 moved_s_idx = dense_to_sparse[last_d_idx];
        dense_to_sparse[d_idx] = moved_s_idx;
        meta[moved_s_idx].idx = d_idx;
    }

    dense.pop_back();
    dense_to_sparse.pop_back();

    meta[id.idx].idx = invalid_index;
    meta[id.idx].gen++;
    free_indices.push_back(id.idx);
}

template <typename T> //
bool Storage<T>::valid(GID id) {
    if (id.idx < meta.size()) {
        return false;
    }
    auto m = meta[id.idx];
    return m.gen == id.gen && m.idx != invalid_index;
}
