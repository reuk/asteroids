#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "logger.h"

template<GLuint type>
class BufferObject {
public:
    BufferObject():
        index(0)
    {
        glGenBuffers(1, &index);
    }

    template<typename T>
    BufferObject(const std::vector<T> & t, GLuint flag) {
        data(t, flag);
    }

    virtual ~BufferObject() {
        glDeleteBuffers(1, &index);
    }

    void bind() const {
        glBindBuffer(type, index);
    }

    static void unbind() {
        glBindBuffer(type, 0);
    }

    template<typename T>
    void data(const std::vector<T> & t, GLuint flag) const {
        bind();
        glBufferData(type, t.size() * sizeof(T), t.data(), flag);
    }

    GLuint get_index() const {
        return index;
    }

private:
    GLuint index;
};

using VBO = BufferObject<GL_ARRAY_BUFFER>;
using IBO = BufferObject<GL_ELEMENT_ARRAY_BUFFER>;
