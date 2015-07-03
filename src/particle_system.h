#pragma once

#include "vao.h"
#include "buffer_object.h"
#include "generic_shader.h"
#include "mover.h"
#include "drawable.h"

#include <glm/glm.hpp>

#include <vector>
#include <random>

class Particle : public Mover<glm::vec2> {
public:
    Particle();

    void update(float dt) override;

    bool is_dead() const;

    float lifetime;
    float lifetime_decrease;

private:
    static glm::vec2 random_angle();

    static std::default_random_engine engine;
    static std::uniform_real_distribution<float> angle_dist;
    static std::uniform_real_distribution<float> speed_dist;
    static std::uniform_real_distribution<float> decrease_dist;
};

class ParticleSystem : public Drawable, public Updatable {
public:
    ParticleSystem(GenericShader &shader_program, const glm::vec2 &position);

    ParticleSystem(const ParticleSystem &rhs) noexcept = delete;
    ParticleSystem &operator=(const ParticleSystem &rhs) noexcept = delete;
    ParticleSystem(ParticleSystem &&rhs) noexcept;
    ParticleSystem &operator=(ParticleSystem &&rhs) noexcept;

    void draw() const override;
    void update(float dt) override;

    bool is_dead() const;

    GenericShader &shader_program;

private:
    void configure_vao() const;

    static const int starting_particles = 100;
    glm::vec2 position;

    std::vector<Particle> particles;

    VAO vao;
    StaticVBO geometry;
    StaticVBO colors;
    StaticIBO ibo;

    int size;
};
