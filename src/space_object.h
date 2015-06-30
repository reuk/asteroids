#pragma once

#include "mover.h"
#include "generic_shader.h"
#include "updatable.h"
#include "drawable.h"

#include <glm/glm.hpp>

class SpaceObject : public Updatable, public Drawable {
   public:
    SpaceObject(StaticDrawable &graphic, float size,
                const Mover<glm::vec2> &position = Mover<glm::vec2>(),
                const Mover<float> &angle = Mover<float>());

    bool inside_boundary() const;
    bool outside_boundary() const;

    void draw() const override;
    void update() override;

    bool is_hit(const SpaceObject &obj) const;

    StaticDrawable *graphic;

    Mover<glm::vec2> position;
    Mover<float> angle;

    float size;

    bool inside;
};
