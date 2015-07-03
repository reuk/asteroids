#include "text_handler.h"

#include "sampler.h"

#include <stdexcept>

using namespace std;
using namespace glm;

TextHandler::TextHandler(TextShader & shader_program)
    : shader_program(shader_program) {
    auto error = FT_Init_FreeType(&library);

    if (error) {
        throw runtime_error("failed to init FreeType");
    }

    auto font_path = "Hyperspace.otf";
    error = FT_New_Face(library, font_path, 0, &face);

    if (error) {
        throw runtime_error("failed to read font");
    }
}

TextHandler::~TextHandler() {
    auto error = FT_Done_Face(face);
    error = FT_Done_FreeType(library);
}

void TextHandler::draw(const string & s, int pixel_height,
                       const vec2 & position) const {
    FT_Set_Pixel_Sizes(face, 0, pixel_height);

    auto pos = position;
    vec2 scale_factor(2.0 / 500);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (auto && i : s) {
        auto error = FT_Load_Char(face, i, FT_LOAD_RENDER);
        if (error) {
            throw runtime_error("failed to load and render glyph");
        }

        Texture tex;
        glActiveTexture(GL_TEXTURE0);
        tex.data(face->glyph->bitmap.width, face->glyph->bitmap.rows,
                 face->glyph->bitmap.buffer);

        Sampler samp;
        samp.parameter_i(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        samp.parameter_i(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        samp.parameter_i(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        samp.parameter_i(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        samp.bind(0);

        glUniform1i(shader_program.get_uniform_location("f_tex"), 0);

        auto v = pos +
                 vec2(face->glyph->bitmap_left, face->glyph->bitmap_top) *
                     scale_factor;
        auto u = vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows) *
                 scale_factor;

        auto vec_geometry = vector<GLfloat>{
            v.x,       v.y, v.x, v.y - u.y, v.x + u.x, v.y,
            v.x + u.x, v.y, v.x, v.y - u.y, v.x + u.x, v.y - u.y};
        auto vec_uv = vector<GLfloat>{0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1};
        auto vec_ibo = vector<GLushort>{0, 1, 2, 3, 4, 5};

        geometry.data(vec_geometry);
        uv.data(vec_uv);
        ibo.data(vec_ibo);

        configure_vao();

        vao.bind();
        glDrawElements(GL_TRIANGLES, vec_ibo.size(), GL_UNSIGNED_SHORT,
                       nullptr);
        VAO::unbind();

        pos += vec2(face->glyph->advance.x, face->glyph->advance.y) *
               scale_factor / 64.0f;
    }
}

void TextHandler::configure_vao() const {
    vao.bind();

    auto v_position = shader_program.get_attrib_location("v_position");
    glEnableVertexAttribArray(v_position);
    geometry.bind();
    glVertexAttribPointer(v_position, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    auto v_uv = shader_program.get_attrib_location("v_uv");
    glEnableVertexAttribArray(v_uv);
    uv.bind();
    glVertexAttribPointer(v_uv, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    ibo.bind();

    VAO::unbind();
}
