#include "ship.h"

#include "key_callback.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

Ship::Ship(ShipGraphic &ship_graphic, BulletGraphic &bullet_graphic)
    : SpaceObject(ship_graphic, 0.1)
    , engine({random_device()()})
    , angle_distribution(0, M_PI * 2)
    , delta_distribution(-0.1, 0.1)
    , bullet_graphic(&bullet_graphic)
    , key_timer({
          {GLFW_KEY_W, make_pair(KeyTimer(), &Ship::forward)},
          {GLFW_KEY_S, make_pair(KeyTimer(), &Ship::backward)},
          {GLFW_KEY_A, make_pair(KeyTimer(), &Ship::left)},
          {GLFW_KEY_D, make_pair(KeyTimer(), &Ship::right)},
      }) {
}

glm::vec2 Ship::forward_vector() const {
    return rotate(vec2(0, 1), angle.get_current());
}

void Ship::forward(float dt) {
    position.impulse(forward_vector() * 0.1f * dt);
}

void Ship::backward(float dt) {
    position.impulse(-forward_vector() * 0.1f * dt);
}

void Ship::left(float dt) {
    angle.impulse(0.2 * dt);
}

void Ship::right(float dt) {
    angle.impulse(-0.2 * dt);
}

void Ship::fire() {
    auto pos = position.get_current() + forward_vector() * 0.1f;
    auto vel = position.get_delta() + forward_vector() * 0.02f;

    auto ang = angle_distribution(engine);
    auto del = delta_distribution(engine);

    listener_list.call(
        &Listener::ship_gun_fired,
        Bullet(*bullet_graphic, Mover<vec2>(pos, vel), Mover<float>(ang, del)));
}

void Ship::add_listener(Listener *listener) {
    listener_list.add(listener);
}
void Ship::remove_listener(Listener *listener) {
    listener_list.remove(listener);
}

void Ship::update(float dt) {
    for (auto &&i : key_timer) {
        if (i.second.first.get_state() == GLFW_PRESS) {
            (this->*(i.second.second))(i.second.first.get_elapsed_time());
            i.second.first.reset();
        }
    }

    SpaceObject::update(dt);
}

void Ship::resize(const vec2 &v) {
}

void Ship::error(const string &s) {
}

void Ship::key(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        key_dispatch<decltype(&Ship::fire)>(this,
                                            {
                                                {GLFW_KEY_SPACE, &Ship::fire},
                                            },
                                            key);
    }

    if (action == GLFW_PRESS || action == GLFW_RELEASE) {
        auto in = key_timer.find(key);
        if (in != key_timer.end()) {
            in->second.first.set_state(action);
            in->second.first.reset();
        }
    }
}
