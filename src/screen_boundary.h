#pragma once

#include "drawable.h"

class ScreenBoundary : public StaticDrawable {
   public:
    ScreenBoundary(GenericShader& shader_program);
    void draw() const override;
};
