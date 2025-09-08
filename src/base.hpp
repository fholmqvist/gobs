#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef uintptr_t uptr;
typedef ptrdiff_t size;
typedef size_t usize;

typedef u64 ID;

std::string frmt(const char* fmt, ...);
std::pair<int, int> aspect_ratio(int width, int height);
