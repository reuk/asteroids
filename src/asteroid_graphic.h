#pragma once

#include "static_drawable.h"

class AsteroidGraphic : public StaticDrawable {
public:
    AsteroidGraphic(GenericShader &shader_program);

private:
    static const int POINTS = 10;
};
