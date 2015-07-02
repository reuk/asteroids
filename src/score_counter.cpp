#include "score_counter.h"

#include "sampler.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdexcept>
#include <string>

using namespace std;
using namespace glm;

ScoreCounter::ScoreCounter(ScoreCounterShader & shader_program,
                           const glm::vec2 & position)
    : shader_program(shader_program)
    , position(position) {
    auto error = FT_Init_FreeType(&library);

    if (error) {
        throw runtime_error("failed to init FreeType");
    }

    auto font_path = "Hyperspace.otf";
    error = FT_New_Face(library, font_path, 0, &face);

    if (error) {
        throw runtime_error("failed to read font");
    }

    FT_Set_Pixel_Sizes(face, 0, 20);

    if (error) {
        throw runtime_error("failed to set font size");
    }
}

ScoreCounter::~ScoreCounter() {
    auto error = FT_Done_Face(face);
    error = FT_Done_FreeType(library);
}

void ScoreCounter::set_score(int s) {
    score = s;
}

void ScoreCounter::draw() const {
    auto score_string = to_string(score);
    stringstream ss;
    ss << setw(8) << score_string;
    score_string = ss.str();

    auto x = position.x;
    auto y = position.y;

    float sx = 2.0 / 500;
    float sy = 2.0 / 500;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (auto && i : score_string) {
        auto error = FT_Load_Char(face, i, FT_LOAD_RENDER);
        if (error) {
            throw runtime_error("failed to load and render glyph");
        }

        Texture tex;
        glActiveTexture(GL_TEXTURE0);
        tex.data(face->glyph->bitmap.width, face->glyph->bitmap.rows, face->glyph->bitmap.buffer);

        Sampler samp;
        samp.parameter_i(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        samp.parameter_i(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        samp.parameter_i(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        samp.parameter_i(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        samp.bind(0);

        glUniform1i(shader_program.get_uniform_location("f_tex"), 0);

        auto vx = x + face->glyph->bitmap_left * sx;
        auto vy = y + face->glyph->bitmap_top * sy;
        auto w = face->glyph->bitmap.width  * sx;
        auto h = face->glyph->bitmap.rows  * sy;

        auto vec_geometry = vector<GLfloat>{vx, vy, 0, vx, vy - h, 0, vx + w, vy, 0, vx + w, vy, 0, vx, vy - h, 0, vx + w, vy - h, 0};
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

        x += (face->glyph->advance.x >> 6) * sx;
        y += (face->glyph->advance.y >> 6) * sy;
    }
}

void ScoreCounter::configure_vao() const {
    vao.bind();

    auto v_position = shader_program.get_attrib_location("v_position");
    glEnableVertexAttribArray(v_position);
    geometry.bind();
    glVertexAttribPointer(v_position, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    auto v_uv = shader_program.get_attrib_location("v_uv");
    glEnableVertexAttribArray(v_uv);
    uv.bind();
    glVertexAttribPointer(v_uv, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    ibo.bind();

    VAO::unbind();
}
