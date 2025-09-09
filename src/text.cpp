#include "text.hpp"

#include "camera.hpp"
#include "constants.hpp"
#include "shader.hpp"

void TextRenderer::init(u32 &existing_texture) {
    assert(existing_texture);

    SHADER_ID = glCreateProgram();
    glUseProgram(SHADER_ID);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    compile_shaders(SHADER_ID, "assets/shaders/text_vs.glsl", "assets/shaders/text_fs.glsl");

    TEXTURE_ID = existing_texture;

    glUseProgram(0);
}

void TextRenderer::render(std::string t, float x, float y, uint8_t size, vec3 color) {
    start(color);

    for (uint16_t i = 0; i < t.size(); i++) {
        render_char(t[i], x + size * i, y, size);
    }

    end();
}

void TextRenderer::render_raw(std::vector<char> chars, float x, float y, uint8_t size, vec3 color) {
    start(color);

    for (uint16_t i = 0; i < chars.size(); i++) {
        render_char(chars[i], x + size * i, y, size);
    }

    end();
}

void TextRenderer::render_one(char c, float x, float y, uint8_t size, vec3 color) {
    start(color);
    render_char(c, x + size, y, size);
    end();
}

void TextRenderer::start(vec3 color) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glUseProgram(SHADER_ID);
    glViewport(0, 0, SCREEN_W, SCREEN_H);

    glBindVertexArray(VAO);

    auto color_loc = glGetUniformLocation(SHADER_ID, "color");
    glUniform3f(color_loc, color.x, color.y, color.z);
}

void TextRenderer::end() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    glUseProgram(0);
}

void TextRenderer::render_char(unsigned char c, float x, float y, u8 size) {
    const auto quad = get_char(c);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    const auto STRIDE = 4 * sizeof(float);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, STRIDE, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, STRIDE, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, TEXTURE_ID);
    glUniform1i(glGetUniformLocation(SHADER_ID, "_texture"), 0);

    auto pos = vec3(x + (float)size / 2, y + (float)size / 2, 0);
    auto model = translate(mat4(1), pos);
    model = scale(model, vec3(size, size, 1));
    auto mvp = ORTHO * model;

    auto mvp_loc = glGetUniformLocation(SHADER_ID, "mvp");
    glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, value_ptr(mvp));

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

std::array<float, 16> TextRenderer::get_char(char c) {
    const int cols = 32;
    const int rows = 32;
    const float gw = 1.0f / cols;
    const float gh = 1.0f / rows;

    int cx = c % cols;
    int cy = c / cols;

    float tx = cx * gw;
    float ty = cy * gh;

    return {
        -0.5f, 0.5f,  tx,      ty + gh, //
        0.5f,  0.5f,  tx + gw, ty + gh, //
        0.5f,  -0.5f, tx + gw, ty,      //
        -0.5f, -0.5f, tx,      ty       //
    };
}
