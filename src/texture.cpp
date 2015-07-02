#include "texture.h"

using namespace std;

Texture::Texture()
    : index(0) {
    glGenTextures(1, &index);
}

Texture::~Texture() {
    glDeleteTextures(1, &index);
}

void Texture::bind() const {
    glBindTexture(GL_TEXTURE_2D, index);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::data(GLsizei w, GLsizei h, GLubyte * t) const {
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0, GL_RED, GL_UNSIGNED_BYTE,
                 t);
}

GLuint Texture::get_index() const {
    return index;
}
