#include "engine.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

Mover::Mover(Program & shader_program):
    shader_program(shader_program),
    angle(0),
    aspect(0)
{
    //  create buffers
    geometry.data(vector<GLfloat>{
             0,  1,  0,
             1, -1,  0,
            -1, -1,  0}, GL_STATIC_DRAW);
    colors.data(vector<GLfloat>{
             1,  0,  0,
             0,  1,  0,
             0,  0,  1}, GL_STATIC_DRAW);
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

}

void Mover::draw() const {
    vao.bind();

    auto size = 0;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

    auto v_mvp = shader_program.get_uniform_location("v_mvp");

    for (auto angle = 0; angle != 10; ++angle) {
        glUniformMatrix4fv(v_mvp, 1, GL_FALSE, value_ptr(mvp * rotate(mat4(1), radians(static_cast<float>(glfwGetTime() * angle)), vec3(0, 1, 1))));
        glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, nullptr);
    }

    VAO::unbind();
}

vec2 Mover::get_position() const { return position; }
vec2 Mover::get_velocity() const { return velocity; }
float Mover::get_angle() const { return angle; }

void Mover::resize(const vec2 & v) {
    aspect = v.x / v.y;
    mvp = perspective(45.0f, aspect, 0.1f, 10.0f) * lookAt(vec3(0, 0, 0), vec3(0, 0, -4), vec3(0, 1, 0)) * translate(mat4(1), vec3(0, 0, -4));
}

void Mover::error(const string & s) {

}

void Mover::key(int key, int scancode, int action, int mods) {
    map<int, string> keys = {
        {GLFW_KEY_W, "forward"},
        {GLFW_KEY_S, "backward"},
        {GLFW_KEY_A, "left"},
        {GLFW_KEY_D, "right"},
        {GLFW_KEY_SPACE, "shoot"},
    };

    map<int, string> actions = {
        {GLFW_PRESS, "start"},
        {GLFW_REPEAT, "continue"},
        {GLFW_RELEASE, "stop"},
    };

    auto d_in = keys.find(key);
    auto a_in = actions.find(action);

    if (d_in != keys.end() && a_in != actions.end())
        Logger::log_err(actions[action], " ", keys[key]);
}

