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

Mover::Mover(Program & shader_program):
    shader_program(shader_program),
    position(0),
    velocity(0),
    angle(0),
    aspect(0)
{
    //  create buffers
    geometry.data(vector<GLfloat>{
             0,  1,  0,
             1, -1,  0,
            -1, -1,  0}, GL_STATIC_DRAW);
    colors.data(vector<GLfloat>{
             0,  1,  1,
             0,  1,  1,
             0,  1,  1}, GL_STATIC_DRAW);
    ibo.data(vector<GLushort>{0, 1, 2}, GL_STATIC_DRAW);

    //  configure vao
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

void Mover::update() {
    position += velocity;

    position = mod(position + 1.0f, 2.0f) - 1.0f;

    angle += dangle;
    angle = fmod(angle, static_cast<float>(M_PI * 2));
}

void Mover::draw() const {
    auto size = 0;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    auto v_mvp = shader_program.get_uniform_location("v_mvp");

    vao.bind();

    glUniformMatrix4fv(v_mvp, 1, GL_FALSE, value_ptr(mvp * translate(mat4(1), vec3(position, 0)) * rotate(mat4(1), angle, vec3(0, 0, 1)) * scale(mat4(1), vec3(0.1))));
    glDrawElements(GL_LINE_LOOP, size / sizeof(GLushort), GL_UNSIGNED_SHORT, nullptr);

    VAO::unbind();
}

vec2 Mover::get_position() const { return position; }
vec2 Mover::get_velocity() const { return velocity; }
float Mover::get_angle() const { return angle; }

void Mover::resize(const vec2 & v) {
    aspect = v.x / v.y;
    mvp = (
            perspective(45.0f, aspect, 0.1f, 10.0f) *
            lookAt(vec3(0, 0, 0), vec3(0, 0, -4), vec3(0, 1, 0)) *
            translate(mat4(1), vec3(0, 0, -4)));
}

void Mover::error(const string & s) {

}

vec2 Mover::forward()   const { return rotate(vec2(0, 1), angle) * 0.01f; }
vec2 Mover::backward()  const { return -forward(); }
float Mover::left()     const { return 0.01f; }
float Mover::right()    const { return -left(); }

void Mover::key(int key, int scancode, int action, int mods) {
    map<int, decltype(&Mover::forward)> thrusters = {
        {GLFW_KEY_W, &Mover::forward},
        {GLFW_KEY_S, &Mover::backward},
    };

    auto thrust_in = thrusters.find(key);
    if (thrust_in != thrusters.end() && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        velocity += (this->*(thrust_in->second))();
    }

    map<int, decltype(&Mover::left)> rotators = {
        {GLFW_KEY_A, &Mover::left},
        {GLFW_KEY_D, &Mover::right},
    };

    auto rotate_in = rotators.find(key);
    if (rotate_in != rotators.end() && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
        dangle += (this->*(rotate_in->second))();
    }
}

