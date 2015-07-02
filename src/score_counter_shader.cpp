#include "score_counter_shader.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

ScoreCounterShader::ScoreCounterShader()
    : ShaderProgram(vertex_shader, fragment_shader) {
}

void ScoreCounterShader::set_matrix(const string &s, const mat4 &mat) const {
    glUniformMatrix4fv(get_uniform_location(s), 1, GL_FALSE, value_ptr(mat));
}

void ScoreCounterShader::set_model_matrix(const mat4 &mat) const {
    set_matrix("v_model", mat);
}

void ScoreCounterShader::set_view_matrix(const mat4 &mat) const {
    set_matrix("v_view", mat);
}

void ScoreCounterShader::set_projection_matrix(const mat4 &mat) const {
    set_matrix("v_projection", mat);
}

const string ScoreCounterShader::vertex_shader(R"(
#version 330
in vec3 v_position;
in vec2 v_uv;
out vec2 f_uv;
void main() {
    gl_Position = vec4(v_position, 1.0);
    f_uv = v_uv;
}
)");

const string ScoreCounterShader::fragment_shader(R"(
#version 330
in vec2 f_uv;
out vec4 frag_color;
uniform sampler2D f_tex;
void main() {
    frag_color = vec4(vec3(1.0), texture(f_tex, f_uv).r) * vec4(0.0, 1.0, 1.0, 1.0);
}
)");
