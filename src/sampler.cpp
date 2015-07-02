#include "sampler.h"

using namespace std;

Sampler::Sampler()
    : index(0) {
    glGenSamplers(1, &index);
}

Sampler::~Sampler() {
    glDeleteSamplers(1, &index);
}

void Sampler::bind(GLuint where) const {
    glBindSampler(where, index);
}

void Sampler::unbind() {
    glBindSampler(GL_TEXTURE_2D, 0);
}

GLuint Sampler::get_index() const {
    return index;
}

void Sampler::parameter_i(GLuint a, GLuint b) const {
    glSamplerParameteri(index, a, b);
}
