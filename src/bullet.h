#pragma once

#include "space_object.h"

class Bullet : public SpaceObject {
   public:
    Bullet(StaticDrawable& bullet_graphic,
           const Mover<glm::vec2>& position = Mover<glm::vec2>(),
           const Mover<float>& angle = Mover<float>());
};
