#pragma once

#include "space_object.h"

class Asteroid: public SpaceObject {
public:
    Asteroid(
            Program & p,
            const Mover<glm::vec2> & position = Mover<glm::vec2>(),
            const Mover<float> & angle = Mover<float>());
private:
    static const int POINTS = 10;
};
