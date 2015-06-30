#pragma once

#include "vao.h"
#include "buffer_object.h"
#include "generic_shader.h"
#include "mover.h"

#include <glm/glm.hpp>

#include <vector>

using Particle = Mover<glm::vec2>;
class Particle : public Mover<glm::vec2> {
public:
    void update() override;

    bool is_dead() const;
private:
    float lifetime;
}

class ParticleSystem: public Drawable, public Updatable {
public:
    void draw() const override;
    void update() override;
private:
    static const int starting_particles = 50;
    vector<Particle> particles;

    VAO vao;
    BufferObject<GL_ARRAY_BUFFER, TODO> vbo;
    BufferObject<GL_ARRAY_BUFFER, TODO> colors;
    BufferObject<GL_ELEMENT_ARRAY_BUFFER, TODO> ibo;
};
