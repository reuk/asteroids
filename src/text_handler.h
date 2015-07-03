#pragma once

#include "text_shader.h"
#include "buffer_object.h"
#include "vao.h"

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

class TextHandler {
public:
    TextHandler(TextShader & shader_program, const glm::vec2 & position);
    virtual ~TextHandler();

    void draw(const std::string & s, int pixel_height) const;
    float measure(const std::string & s, int pixel_height) const;

private:
    void configure_vao() const;

    TextShader & shader_program;
    glm::vec2 position;

    FT_Library library;
    FT_Face face;

    VAO vao;
    DynamicVBO geometry;
    DynamicVBO uv;
    DynamicIBO ibo;
};
