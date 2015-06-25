#include "engine.h"

#include "key_callback.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <vector>

using namespace std;
using namespace glm;

SpaceObject::SpaceObject(
        Program & shader_program,
        const vector<GLfloat> & g,
        const vector<GLfloat> & c,
        const vector<GLushort> & i,
        float size,
        const Mover<vec2> & position,
        const Mover<float> & angle):
    StaticDrawable(shader_program, g, c, i),
    shader_program(shader_program),
    position(position),
    angle(angle),
    size(size)
{

}

void SpaceObject::draw() const {
    auto scale_matrix = scale(mat4(1), vec3(size));
    auto angle_matrix = rotate(mat4(1), angle.get_current(), vec3(0, 0, 1));
    auto position_matrix = translate(mat4(1), vec3(position.get_current(), 0.0f));
    auto model_matrix = position_matrix * angle_matrix * scale_matrix;

    glUniformMatrix4fv(shader_program.get_uniform_location("v_model"), 1, GL_FALSE, value_ptr(model_matrix));
    StaticDrawable::draw();
}

void SpaceObject::update() {
    position.update();
    angle.update();

    position.set_current(mod(position.get_current() + 1.0f, 2.0f) - 1.0f);
    angle.set_current(fmod(angle.get_current(), static_cast<float>(M_PI * 2)));
}

Ship::Ship(Program & shader_program):
    SpaceObject(
            shader_program,
            vector<GLfloat>{
                     0,  1,  0,
                     1, -1,  0,
                    -1, -1,  0},
            vector<GLfloat>{
                     0,  1,  1,
                     0,  1,  1,
                     0,  1,  1},
            vector<GLushort>{0, 1, 2},
            0.1),
    engine({random_device()()}),
    angle_distribution(0, M_PI * 2),
    delta_distribution(-0.1, 0.1)
{

}

vec2 Ship::forward()   const { return rotate(vec2(0, 1), angle.get_current()) * 0.01f; }
vec2 Ship::backward()  const { return -forward(); }
float Ship::left()     const { return 0.02f; }
float Ship::right()    const { return -left(); }

void Ship::fire() {
    auto pos = position.get_current();
    auto vel = position.get_delta() + forward() * 2.0f;

    auto ang = angle_distribution(engine);
    auto del = delta_distribution(engine);

    listener_list.call(&Listener::ship_gun_fired, Bullet(shader_program, Mover<vec2>(pos, vel), Mover<float>(ang, del)));
}

void Ship::add_listener(Listener * listener) { listener_list.add(listener); }
void Ship::remove_listener(Listener * listener) { listener_list.remove(listener); }

void Ship::resize(const vec2 & v) {}
void Ship::error(const string & s) {}
void Ship::key(int key, int scancode, int action, int mods) {
    if (! (action == GLFW_PRESS || action == GLFW_REPEAT))
        return;

    key_dispatch<decltype(&Ship::forward)>(this, {
        {GLFW_KEY_W, &Ship::forward},
        {GLFW_KEY_S, &Ship::backward},
    }, key, [this](auto i){position.impulse(i);});

    key_dispatch<decltype(&Ship::left)>(this, {
        {GLFW_KEY_A, &Ship::left},
        {GLFW_KEY_D, &Ship::right},
    }, key, [this](auto i){angle.impulse(i);});

    key_dispatch<decltype(&Ship::fire)>(this, {
        {GLFW_KEY_SPACE, &Ship::fire},
    }, key);
}

Bullet::Bullet(
        Program & shader_program,
        const Mover<vec2> & position,
        const Mover<float> & angle):
    SpaceObject(
            shader_program,
            vector<GLfloat>{
                    -1, -1,  0,
                    -1,  1,  0,
                     1,  1,  0,
                     1, -1,  0},
            vector<GLfloat>{
                    0, 0.7, 0.7,
                    0, 0.7, 0.7,
                    0, 0.7, 0.7,
                    0, 0.7, 0.7},
            vector<GLushort>{0, 1, 2, 3},
            0.01,
            position,
            angle)
{

}

vector<GLushort> indices(int points) {
    vector<GLushort> ret(points);
    auto i = 0;
    generate(begin(ret), end(ret), [&i]() { return i++; });
    return ret;
}

vector<vec3> noise_circle(int points) {
    default_random_engine engine{random_device()()};
    normal_distribution<float> noise_dist(1.0, 0.1);

    auto counter = indices(points);

    vector<float> angles(points);
    transform(
            begin(counter),
            end(counter),
            begin(angles),
            [points](auto i) {
                return (i++ * 2 * M_PI) / points;
            });

    vector<vec3> ret(points);
    transform(
            begin(angles),
            end(angles),
            begin(ret),
            [&engine, &noise_dist](auto i) {
                auto radius = noise_dist(engine);
                return vec3(sin(i), cos(i), 0) * radius;
            });
    return ret;
}

vector<GLfloat> format(const vector<vec3> & points) {
    vector<GLfloat> ret(points.size() * 3);
    for (auto i = 0; i != points.size(); ++i) {
        auto ret_index = i * 3;
        ret[ret_index + 0] = points[i].x;
        ret[ret_index + 1] = points[i].y;
        ret[ret_index + 2] = points[i].z;
    }
    return ret;
}

Asteroid::Asteroid(
        Program & shader_program,
        const Mover<vec2> & position,
        const Mover<float> & angle):
    SpaceObject(
            shader_program,
            format(noise_circle(POINTS)),
            format(vector<vec3>(POINTS, vec3(0, 1, 1))),
            indices(POINTS),
            0.1,
            position,
            angle)
{

}
