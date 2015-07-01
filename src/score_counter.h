#pragma once

#include "drawable.h"

#include <glm/glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

class ScoreCounter : public Drawable {
public:
    ScoreCounter();

private:
    FT_Library library;
    FT_Face face;
};
