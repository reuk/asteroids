#pragma once

#include "ship_graphic.h"

class LifeCounter : public Drawable {
public:
  LifeCounter(StaticDrawable &ship_graphic, float size,
              const glm::vec2 &position);

  void draw() const override;

  void set_lives(int lives);

  StaticDrawable *graphic;
  float size;
  glm::vec2 position;
  int lives;
};
