#pragma once

#include "text_shader.h"
#include "buffer_object.h"
#include "vao.h"

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

class TextHandler {
public:
    TextHandler(TextShader & shader_program);
    virtual ~TextHandler();

    void draw(const std::string & s, int pixel_height,
              const glm::vec2 & position) const;
    float measure(const std::string & s, int pixel_height) const;

private:
    void configure_vao() const;

    TextShader & shader_program;

    FT_Library library;
    FT_Face face;

    VAO vao;
    DynamicVBO geometry;
    DynamicVBO uv;
    DynamicIBO ibo;
};
