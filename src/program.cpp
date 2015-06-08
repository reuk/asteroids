#include "program.h"

using namespace std;

Program::Program():
    index(glCreateProgram())
{
    if (index == 0) {
        throw runtime_error("failed to create shader program");
    }
}

Program::~Program() {
    if (index)
        glDeleteProgram(index);
}

void Program::attach(const Shader & s) const {
    glAttachShader(index, s.get_index());
}

void Program::detach(const Shader & s) const {
    glDetachShader(index, s.get_index());
}

void Program::link() const {
    glLinkProgram(index);
}

void Program::use() const {
    glUseProgram(index);
}

GLuint Program::get_index() const {
    return index;
}
