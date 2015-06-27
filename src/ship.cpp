#include "ship.h"

#include "key_callback.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

Ship::Ship(GenericShader & shader_program):
    SpaceObject(
            shader_program,
            vector<GLfloat>{
                     0.0,  1.0,  0,
                     1.0, -1.0,  0,
                     0.5, -0.5,  0,
                    -0.5, -0.5,  0,
                    -1.0, -1.0,  0},
            vector<GLfloat>{
                     0,  1,  1,
                     0,  1,  1,
                     0,  1,  1,
                     0,  1,  1,
                     0,  1,  1},
            vector<GLushort>{0, 1, 2, 3, 4},
            0.1),
    engine({random_device()()}),
    angle_distribution(0, M_PI * 2),
    delta_distribution(-0.1, 0.1)
{

}

glm::vec2 Ship::forward_vector() const {
    return rotate(vec2(0, 1), angle.get_current());
}

vec2 Ship::forward()   const { return forward_vector() * 0.01f; }
vec2 Ship::backward()  const { return -forward(); }
float Ship::left()     const { return 0.02f; }
float Ship::right()    const { return -left(); }

void Ship::fire() {
    auto pos = position.get_current() + forward_vector() * 0.1f;
    auto vel = position.get_delta() + forward_vector() * 0.02f;

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

void Ship::set_life(float l) {life = l;}
float Ship::get_life() const {return life;}

