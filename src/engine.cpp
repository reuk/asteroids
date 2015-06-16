#include "engine.h"

#include "logger.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

Mover::Mover():
    angle(0),
    aspect(1)
{
    //  load shaders
    Shader fs(GL_FRAGMENT_SHADER);
    fs.source(fragment_shader);
    fs.compile();

    Shader vs(GL_VERTEX_SHADER);
    vs.source(vertex_shader);
    vs.compile();

    shader_program.attach(fs);
    shader_program.attach(vs);
    shader_program.link();

    shader_program.check();
    shader_program.verify();

    //  create buffers
    geometry.data(vector<GLfloat>{
            -1, -1,  1,
             1, -1,  1,
             1,  1,  1,
            -1,  1,  1,
            -1, -1, -1,
             1, -1, -1,
             1,  1, -1,
            -1,  1, -1}, GL_STATIC_DRAW);
    colors.data(vector<GLfloat>{
            1, 0, 0,
            0, 1, 0,
            0, 0, 1,
            1, 1, 1,
            1, 0, 0,
            0, 1, 0,
            0, 0, 1,
            1, 1, 1}, GL_STATIC_DRAW);
    ibo.data(vector<GLushort>{
            0, 1, 2,
            2, 3, 0,
            3, 2, 6,
            6, 7, 3,
            7, 6, 5,
            5, 4, 7,
            4, 5, 1,
            1, 0, 4,
            4, 0, 3,
            3, 7, 4,
            1, 5, 6,
            6, 2, 1}, GL_STATIC_DRAW);

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

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mover::update() {
    auto current_seconds = glfwGetTime();
    float angle = current_seconds * 45;

    vec3 axis_y(0, 1, 0);
    auto anim = rotate(mat4(1.0), radians(angle), axis_y);

    auto model = translate(mat4(1.0), vec3(0.0, 0.0, -4.0));
    auto view = lookAt(vec3(0.0, 2.0, 0.0), vec3(0.0, 0.0, -4.0), vec3(0.0, 1.0, 0.0));
    auto projection = perspective(45.0f, aspect, 0.1f, 10.0f);

    auto mvp = projection * view * model * anim;

    shader_program.use();
    glUniformMatrix4fv(shader_program.get_uniform_location("v_mvp"), 1, GL_FALSE, value_ptr(mvp));
    glUseProgram(0);
}

void Mover::draw() const {
    shader_program.use();

    vao.bind();
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

    glBindVertexArray(0);
    glUseProgram(0);
}

vec2 Mover::get_position() const { return position; }
vec2 Mover::get_velocity() const { return velocity; }
float Mover::get_angle() const { return angle; }

void Mover::resize(const vec2 & v) {
    aspect = v.x / v.y;
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

const string Mover::vertex_shader(R"(
#version 400
in vec3 v_position;
in vec3 v_color;
out vec3 f_color;
uniform mat4 v_mvp;
void main() {
    gl_Position = v_mvp * vec4(v_position, 1.0);
    f_color = v_color;
}
)");

const string Mover::fragment_shader(R"(
#version 400
in vec3 f_color;
out vec4 frag_color;
void main() {
    frag_color = vec4(f_color, 1.0);
}
)");
