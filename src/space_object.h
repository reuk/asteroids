#pragma once

#include "mover.h"
#include "program.h"
#include "updatable.h"
#include "drawable.h"

#include <glm/glm.hpp>

class SpaceObject: public Updatable, public StaticDrawable {
public:
    SpaceObject(
            Program & shader_program,
            const std::vector<GLfloat> &,
            const std::vector<GLfloat> &,
            const std::vector<GLushort> &,
            float size,
            const Mover<glm::vec2> & position = Mover<glm::vec2>(),
            const Mover<float> & angle = Mover<float>());

    void draw() const override;
    void update() override;

    Program & shader_program;

    Mover<glm::vec2> position;
    Mover<float> angle;

    const float size;
};

