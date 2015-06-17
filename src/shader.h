#pragma once

#include <GLFW/glfw3.h>

#include <string>
#include <stdexcept>

template<GLuint type>
class Shader {
public:
    Shader():
        index(glCreateShader(type))
    {
        if (index == 0) {
            throw std::runtime_error("failed to create shader");
        }
    }

    virtual ~Shader() {
        glDeleteShader(index);
    }

    void source(const std::string & src) const {
        auto ptr = src.c_str();
        glShaderSource(index, 1, &ptr, nullptr);
    }

    void compile() const {
        glCompileShader(index);
    }

    GLuint get_index() const {
        return index;
    }
private:
    GLuint index;
};

using VertexShader = Shader<GL_VERTEX_SHADER>;
using FragmentShader = Shader<GL_FRAGMENT_SHADER>;
