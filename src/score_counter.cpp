#include "score_counter.h"

#include <stdexcept>
#include <string>

using namespace std;
using namespace glm;

ScoreCounter::ScoreCounter() {
    auto error = FT_Init_FreeType(&library);

    if (error) {
        throw runtime_error("failed to init FreeType");
    }

    error = FT_New_Face(library, "~/Library/Fonts/Hyperspace.otf", 0, &face);

    if (error) {
        throw runtime_error("failed to read font");
    }

    //  TODO set last two fields based on actual screen resolution
    error = FT_Set_Char_Size(face, 0, 16 * 64, 1000, 1000);

    string test_string = "hello world";

    auto pen_x = 0;
    auto pen_y = 0;

    for (auto && i : test_string) {
        error = FT_Load_Char(face, i, FT_LOAD_RENDER);
        if (error) {
            throw runtime_error("failed to load and render glyph");
        }

        //  TODO draw to target surface
        //  render_glyph(&face->glyph->bitmap, pen_x + face->glyph->bitmap_left,
        //  pen_y + face->glyph->bitmap_top);

        pen_x += face->glyph->advance.x >> 6;
        pen_y += face->glyph->advance.y >> 6;
    }
}
