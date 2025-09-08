#pragma once

#include "pch.hpp" // IWYU pragma: keep

// For making another bitmap font in the future:
// https://www.ascii-code.com/

const auto DEFAULT_FONT_SIZE = 16;

struct TextRenderer {
  private:
    u32 SHADER_ID;
    u32 TEXTURE_ID;
    u32 VAO;
    u32 VBO;
    u32 EBO;

    static constexpr u32 indices[] = { 0, 1, 2, 2, 3, 0 };

  public:
    TextRenderer() {
    }

    void init(u32 &existing_texture);
    void render(std::string t, float x, float y, uint8_t size, vec3 color = vec3(1));
    void render_raw(std::vector<char> chars, float x, float y, uint8_t size, vec3 color = vec3(1));
    void render_one(char c, float x, float y, uint8_t size, vec3 color = vec3(1));

  private:
    void start(vec3 color);
    void end();
    void render_char(unsigned char c, float x, float y, uint8_t size);
    std::array<float, 16> get_char(char c);
};
