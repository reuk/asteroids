#pragma once

#include "score_counter_shader.h"

#include "drawable.h"

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

class ScoreCounter : public Drawable {
public:
    ScoreCounter(ScoreCounterShader & shader_program,
                 const glm::vec2 & position);
    virtual ~ScoreCounter();

    void draw() const override;
    void render_glyph(FT_Bitmap * bitmap, int x, int y) const;

    void set_score(int s);

private:
    void configure_vao() const;

    ScoreCounterShader & shader_program;
    glm::vec2 position;

    FT_Library library;
    FT_Face face;

    int score;

    VAO vao;
    BufferObject<GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW> geometry;
    BufferObject<GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW> uv;
    BufferObject<GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW> ibo;
};
