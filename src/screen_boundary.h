#pragma once

#include "drawable.h"

class ScreenBoundary: public StaticDrawable {
public:
    ScreenBoundary(Program & shader_program);
    void draw() const override;
};
