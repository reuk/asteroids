#include "buffer_object.h"

BufferObject::BufferObject(GLuint type):
    type(type),
    index(0)
{
    glGenBuffers(1, &index);
}

BufferObject::~BufferObject() {
    glDeleteBuffers(1, &index);
}

void BufferObject::bind() const {
    glBindBuffer(type, index);
}

GLuint BufferObject::get_index() const {
    return index;
}

VBO::VBO():
    BufferObject(GL_ARRAY_BUFFER)
{

}

IBO::IBO():
    BufferObject(GL_ELEMENT_ARRAY_BUFFER)
{

}
