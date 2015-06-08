#include "shader.h"

using namespace std;

Shader::Shader(GLuint type):
    index(glCreateShader(type))
{
    if (index == 0) {
        throw runtime_error("failed to create shader");
    }
}

Shader::~Shader() {
    glDeleteShader(index);
}

void Shader::source(const string & src) const {
    const auto ptr = src.c_str();
    glShaderSource(index, 1, &ptr, nullptr);
}

void Shader::compile() const {
    glCompileShader(index);
}

GLuint Shader::get_index() const {
    return index;
}
