#pragma once

#include "space_object.h"

class Bullet: public SpaceObject {
public:
    Bullet(
            Program & p,
            const Mover<glm::vec2> & position = Mover<glm::vec2>(),
            const Mover<float> & angle = Mover<float>());
};

