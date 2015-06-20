#include "engine.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

Ship::Ship(Program & shader_program):
    StaticDrawable(
            shader_program,
            vector<GLfloat>{
                     0,  1,  0,
                     1, -1,  0,
                    -1, -1,  0},
            vector<GLfloat>{
                     0,  1,  1,
                     0,  1,  1,
                     0,  1,  1},
            vector<GLushort>{0, 1, 2}),
    shader_program(shader_program)
{

}

Ship::~Ship() = default;

void Ship::update() {
    position.update();
    angle.update();

    position.set_current(mod(position.get_current() + 1.0f, 2.0f) - 1.0f);
    angle.set_current(fmod(angle.get_current(), static_cast<float>(M_PI * 2)));
}

void Ship::draw() const {
    auto scale_matrix = scale(mat4(1), vec3(0.1));
    auto angle_matrix = rotate(mat4(1), angle.get_current(), vec3(0, 0, 1));
    auto position_matrix = translate(mat4(1), vec3(position.get_current(), 0.0f));
    auto model_matrix = position_matrix * angle_matrix * scale_matrix;

    glUniformMatrix4fv(shader_program.get_uniform_location("v_model"), 1, GL_FALSE, value_ptr(model_matrix));
    StaticDrawable::draw();
}

void Ship::resize(const vec2 & v) {
    aspect = v.x / v.y;
    viewport = vec4(0, 0, v.x, v.y);
}

void Ship::error(const string & s) {}

vec2 Ship::forward()   const { return rotate(vec2(0, 1), angle.get_current()) * 0.01f; }
vec2 Ship::backward()  const { return -forward(); }
float Ship::left()     const { return 0.01f; }
float Ship::right()    const { return -left(); }

void Ship::key(int key, int scancode, int action, int mods) {
    map<int, decltype(&Ship::forward)> thrusters = {
        {GLFW_KEY_W, &Ship::forward},
        {GLFW_KEY_S, &Ship::backward},
    };

    auto thrust_in = thrusters.find(key);
    if (thrust_in != thrusters.end() && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        position.impulse((this->*(thrust_in->second))());
    }

    map<int, decltype(&Ship::left)> rotators = {
        {GLFW_KEY_A, &Ship::left},
        {GLFW_KEY_D, &Ship::right},
    };

    auto rotate_in = rotators.find(key);
    if (rotate_in != rotators.end() && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        angle.impulse((this->*(rotate_in->second))());
    }
}
