#pragma once

#include "pch.hpp" // IWYU pragma: keep

#include "base.hpp"

struct GID {
    u32 gen;
    u32 idx;
};

struct Meta {
    u32 gen;
    u32 idx;
};

template <typename T> struct Storage {
  private:
    std::vector<u32> dense_to_sparse;
    std::vector<u32> free_indices;
    std::vector<Meta> meta;

    static const u32 invalid_index = -1;

  public:
    std::vector<T> values;

    GID add(T value) {
        u32 s_idx;
        if (!free_indices.empty()) {
            s_idx = free_indices.back();
            free_indices.pop_back();
        } else {
            s_idx = static_cast<u32>(meta.size());
            meta.push_back({ 0, invalid_index });
        }

        u32 d_idx = static_cast<u32>(values.size());
        values.push_back(std::move(value));
        dense_to_sparse.push_back(s_idx);

        meta[s_idx].idx = d_idx;

        return GID{ meta[s_idx].gen, s_idx };
    }

    T get(GID id) {
        assert(valid(id));
        return values[meta[id.idx].idx];
    }

    T* get_ptr(GID id) {
        if (!valid(id)) {
            return nullptr;
        } else {
            return &values[meta[id.idx].idx];
        }
    }

    void remove(GID id) {
        if (!valid(id)) {
            return;
        }

        u32 d_idx = meta[id.idx].idx;
        u32 last_d_idx = static_cast<u32>(values.size() - 1);

        if (d_idx != last_d_idx) {
            std::swap(values[d_idx], values[last_d_idx]);
            u32 moved_s_idx = dense_to_sparse[last_d_idx];
            dense_to_sparse[d_idx] = moved_s_idx;
            meta[moved_s_idx].idx = d_idx;
        }

        values.pop_back();
        dense_to_sparse.pop_back();

        meta[id.idx].idx = invalid_index;
        meta[id.idx].gen++;
        free_indices.push_back(id.idx);
    }

    bool valid(GID id) {
        if (id.idx >= meta.size()) {
            return false;
        }
        auto m = meta[id.idx];
        return m.gen == id.gen && m.idx != invalid_index;
    }
};
