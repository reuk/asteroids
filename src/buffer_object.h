#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "logger.h"

class BufferObject {
public:
    BufferObject(GLuint type);
    virtual ~BufferObject();

    void bind() const;

    template<typename T>
    void data(const std::vector<T> & t, GLuint flag) const {
        bind();
        glBufferData(GL_ARRAY_BUFFER, t.size() * sizeof(T), t.data(), flag);
    }

    GLuint get_index() const;

private:
    GLuint type;
    GLuint index;
};

class VBO: public BufferObject {
public:
    VBO();
};

class IBO: public BufferObject {
public:
    IBO();
};
