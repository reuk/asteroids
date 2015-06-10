#include "engine.h"

#include "logger.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>
#include <string>
#include <vector>

using namespace std;
using namespace glm;

Mover::Mover()
{
    vao.bind();

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

    if (! shader_program.verify()) {
        throw runtime_error("shader program failed to verify");
    }

    //  load geometry and other buffers
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

    auto coord3d = shader_program.get_attrib_location("coord3d");
    glEnableVertexAttribArray(coord3d);
    geometry.bind();
    glVertexAttribPointer(coord3d, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    auto v_color = shader_program.get_attrib_location("v_color");
    glEnableVertexAttribArray(v_color);
    colors.bind();
    glVertexAttribPointer(v_color, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    ibo.bind();
}

void Mover::update() {

}

void Mover::draw() const {
    shader_program.use();
    vao.bind();
    auto size = 0;
    glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
    glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);
}

vec2 Mover::get_position() const { return position; }
vec2 Mover::get_velocity() const { return velocity; }
float Mover::get_angle() const { return angle; }

void Mover::resize(const vec2 & v) {

}

void Mover::error(const string & s) {

}

void Mover::key(int key, int scancode, int action, int mods) {
    map<int, string> directions = {
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

    Logger::log_err(actions[action], " ", directions[key]);
}

const string Mover::vertex_shader(R"(
#version 400
in vec3 coord3d;
in vec3 v_color;
out vec3 f_color;
void main() {
    gl_Position = vec4(coord3d, 1.0);
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
