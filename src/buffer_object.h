#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include "logger.h"

template <GLuint type, GLuint mode>
class BufferObject {
   public:
    BufferObject() : index(0) { glGenBuffers(1, &index); }

    virtual ~BufferObject() { glDeleteBuffers(1, &index); }

    BufferObject(const BufferObject& rhs) noexcept : index(0) {
        glGenBuffers(1, &index);

        auto size = 0;

        glBindBuffer(GL_COPY_READ_BUFFER, rhs.index);
        glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);

        glBindBuffer(GL_COPY_WRITE_BUFFER, index);
        glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, mode);

        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
                            size);

        glBindBuffer(GL_COPY_READ_BUFFER, 0);
        glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
    }

    BufferObject& operator=(const BufferObject& rhs) noexcept {
        index = 0;

        glGenBuffers(1, &index);

        auto size = 0;

        glBindBuffer(GL_COPY_READ_BUFFER, rhs.index);
        glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);

        glBindBuffer(GL_COPY_WRITE_BUFFER, index);
        glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, mode);

        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
                            size);

        glBindBuffer(GL_COPY_READ_BUFFER, 0);
        glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

        return *this;
    }

    BufferObject(BufferObject&& rhs) noexcept : index(0) {
        glGenBuffers(1, &index);

        auto size = 0;

        glBindBuffer(GL_COPY_READ_BUFFER, rhs.index);
        glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);

        glBindBuffer(GL_COPY_WRITE_BUFFER, index);
        glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, mode);

        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
                            size);

        glBindBuffer(GL_COPY_READ_BUFFER, 0);
        glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
    }

    BufferObject& operator=(BufferObject&& rhs) noexcept {
        index = 0;

        glGenBuffers(1, &index);

        auto size = 0;

        glBindBuffer(GL_COPY_READ_BUFFER, rhs.index);
        glGetBufferParameteriv(GL_COPY_READ_BUFFER, GL_BUFFER_SIZE, &size);

        glBindBuffer(GL_COPY_WRITE_BUFFER, index);
        glBufferData(GL_COPY_WRITE_BUFFER, size, nullptr, mode);

        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, 0,
                            size);

        glBindBuffer(GL_COPY_READ_BUFFER, 0);
        glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

        return *this;
    }

    void bind() const { glBindBuffer(type, index); }

    static void unbind() { glBindBuffer(type, 0); }

    template <typename T>
    void data(const std::vector<T>& t) const {
        bind();
        glBufferData(type, t.size() * sizeof(T), t.data(), mode);
    }

    GLuint get_index() const { return index; }

   private:
    GLuint index;
};

using VBO = BufferObject<GL_ARRAY_BUFFER, GL_STATIC_DRAW>;
using IBO = BufferObject<GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW>;
