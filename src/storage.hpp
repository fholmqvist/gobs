#pragma once

#include "base.hpp"
#include "pch.hpp" // IWYU pragma: keep

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
    std::vector<T> dense;
    std::vector<u32> dense_to_sparse;
    std::vector<u32> free_indices;
    std::vector<Meta> meta;

    static const u32 invalid_index = -1;

  public:
    GID add(T);
    T &get(GID);
    void remove(GID);
    bool valid(GID);
};
