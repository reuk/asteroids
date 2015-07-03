#pragma once

#include "static_drawable.h"

class ScreenBoundary : public StaticDrawable {
public:
    ScreenBoundary(GenericShader &shader_program);
    void draw() const override;
};
