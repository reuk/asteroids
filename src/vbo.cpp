#include "vbo.h"

VBO::VBO():
    index(0)
{
    glGenBuffers(1, &index);
}

VBO::~VBO() {
    glDeleteBuffers(1, &index);
}

void VBO::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, index);
}

GLuint VBO::get_index() const {
    return index;
}
