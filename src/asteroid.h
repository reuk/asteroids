#pragma once

#include "space_object.h"
#include "listener_list.h"
#include "bullet.h"
#include "asteroid_graphic.h"

class Asteroid : public SpaceObject {
public:
    Asteroid(StaticDrawable &p,
             const Mover<glm::vec2> &position = Mover<glm::vec2>(),
             const Mover<float> &angle = Mover<float>(), float size = 0.1);

    std::vector<Asteroid> get_fragments() const;

private:
    static const int POINTS = 10;
};
