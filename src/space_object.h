#pragma once

#include "mover.h"
#include "generic_shader.h"
#include "updatable.h"
#include "drawable.h"

#include <glm/glm.hpp>

class SpaceObject : public Updatable, public StaticDrawable {
   public:
    SpaceObject(GenericShader &shader_program, const std::vector<GLfloat> &,
                const std::vector<GLfloat> &, const std::vector<GLushort> &,
                float size,
                const Mover<glm::vec2> &position = Mover<glm::vec2>(),
                const Mover<float> &angle = Mover<float>());

    void draw() const override;
    void update() override;

    bool is_hit(const SpaceObject &obj) const;

    Mover<glm::vec2> position;
    Mover<float> angle;

    float size;
};
