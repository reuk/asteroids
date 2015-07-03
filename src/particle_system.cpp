#include "particle_system.h"

#include "geometry_helpers.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <algorithm>

using namespace std;
using namespace glm;

//  PARTICLE

vec2 Particle::random_angle() {
    auto angle = angle_dist(engine);
    return vec2(sin(angle), cos(angle));
}

Particle::Particle()
    : Mover(vec2(0.0f), random_angle() * speed_dist(engine))
    , lifetime(1.0f)
    , lifetime_decrease(decrease_dist(engine)) {
}

void Particle::update(float dt) {
    lifetime -= lifetime_decrease * dt;
    Mover::update(dt);
}

bool Particle::is_dead() const {
    return lifetime < 0.0f;
}

default_random_engine Particle::engine{random_device()()};
uniform_real_distribution<float> Particle::angle_dist(0, 2 * M_PI);
uniform_real_distribution<float> Particle::speed_dist(0, 0.05);
uniform_real_distribution<float> Particle::decrease_dist(0, 0.02);

//  PARTICLE SYSTEM

ParticleSystem::ParticleSystem(GenericShader &shader_program,
                               const vec2 &position)
    : shader_program(shader_program)
    , position(position)
    , particles(starting_particles)
    , size(starting_particles) {
    for (auto &&i : particles) {
        i = Particle();
    }
    configure_vao();
}

ParticleSystem::ParticleSystem(ParticleSystem &&rhs) noexcept
    : shader_program(rhs.shader_program),
      position(move(rhs.position)),
      particles(move(rhs.particles)),
      geometry(move(rhs.geometry)),
      colors(move(rhs.colors)),
      ibo(move(rhs.ibo)),
      size(rhs.size) {
    configure_vao();
}

ParticleSystem &ParticleSystem::operator=(ParticleSystem &&rhs) noexcept {
    shader_program = move(rhs.shader_program);
    position = move(rhs.position), particles = move(rhs.particles),
    geometry = move(rhs.geometry);
    colors = move(rhs.colors);
    ibo = move(rhs.ibo);
    size = rhs.size;

    configure_vao();

    return *this;
}

void ParticleSystem::draw() const {
    const auto scaling = 0.1;
    auto scale_matrix = scale(mat4(1), vec3(scaling));
    auto position_matrix = translate(mat4(1), vec3(position, 0.0f));
    auto model_matrix = position_matrix * scale_matrix;

    shader_program.set_model_matrix(model_matrix);

    vao.bind();
    glDrawElements(GL_POINTS, size, GL_UNSIGNED_SHORT, nullptr);
    VAO::unbind();
}

vector<vec3> point_geometry(const vector<Particle> &v) {
    vector<vec3> ret(v.size());
    transform(v.begin(), v.end(), ret.begin(),
              [](auto i) { return vec3(i.get_current(), 0.0f); });
    return ret;
}

vector<vec3> point_colors(const vector<Particle> &v) {
    vector<vec3> ret(v.size());
    transform(v.begin(), v.end(), ret.begin(),
              [](auto i) { return vec3(0.0f, i.lifetime, i.lifetime); });
    return ret;
}

void ParticleSystem::update(float dt) {
    for (auto i = particles.begin(); i != particles.end();) {
        i->update(dt);

        if (i->is_dead())
            i = particles.erase(i);
        else
            ++i;
    }

    size = particles.size();

    geometry.data(format(point_geometry(particles)));
    colors.data(format(point_colors(particles)));
    ibo.data(indices(size));
}

bool ParticleSystem::is_dead() const {
    return particles.empty();
}

void ParticleSystem::configure_vao() const {
    vao.bind();

    auto v_position = shader_program.get_attrib_location("v_position");
    glEnableVertexAttribArray(v_position);
    geometry.bind();
    glVertexAttribPointer(v_position, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    auto v_color = shader_program.get_attrib_location("v_color");
    glEnableVertexAttribArray(v_color);
    colors.bind();
    glVertexAttribPointer(v_color, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    ibo.bind();

    VAO::unbind();
}
