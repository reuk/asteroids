#include "text_shader.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

TextShader::TextShader()
    : ShaderProgram(vertex_shader, fragment_shader) {
}

void TextShader::set_matrix(const string &s, const mat4 &mat) const {
    glUniformMatrix4fv(get_uniform_location(s), 1, GL_FALSE, value_ptr(mat));
}

void TextShader::set_model_matrix(const mat4 &mat) const {
    set_matrix("v_model", mat);
}

void TextShader::set_view_matrix(const mat4 &mat) const {
    set_matrix("v_view", mat);
}

void TextShader::set_projection_matrix(const mat4 &mat) const {
    set_matrix("v_projection", mat);
}

const string TextShader::vertex_shader(R"(
#version 330
in vec2 v_position;
in vec2 v_uv;
out vec2 f_uv;
void main() {
    gl_Position = vec4(v_position, 0.0, 1.0);
    f_uv = v_uv;
}
)");

const string TextShader::fragment_shader(R"(
#version 330
in vec2 f_uv;
out vec4 frag_color;
uniform sampler2D f_tex;
void main() {
    frag_color = vec4(vec3(1.0), texture(f_tex, f_uv).r) * vec4(0.0, 1.0, 1.0, 1.0);
}
)");
