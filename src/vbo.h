#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "logger.h"

class VBO {
public:
    VBO();
    virtual ~VBO();

    void bind() const;

    template<typename T>
    void data(const std::vector<T> & t, GLuint flag) const {
        glBufferData(GL_ARRAY_BUFFER, t.size() * sizeof(T), t.data(), flag);
    }

    GLuint get_index() const;

private:
    GLuint index;
};
