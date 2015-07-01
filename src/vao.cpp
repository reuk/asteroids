#include "vao.h"
#include "logger.h"

VAO::VAO()
    : index(0) {
    glGenVertexArrays(1, &index);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &index);
}

void VAO::bind() const {
    glBindVertexArray(index);
}

void VAO::unbind() {
    glBindVertexArray(0);
}

GLuint VAO::get_index() const {
    return index;
}
