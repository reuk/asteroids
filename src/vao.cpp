#include "vao.h"
#include "logger.h"

VAO::VAO():
    index(0)
{
    glGenVertexArrays(1, &index);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &index);
}

void VAO::bind() const {
    glBindVertexArray(index);
}

GLuint VAO::get_index() const {
    return index;
}
