#pragma once

#include <GLFW/glfw3.h>

#include <string>

class Shader {
public:
    Shader(GLuint type);
    virtual ~Shader();

    void source(const std::string & src) const;
    void compile() const;

    GLuint get_index() const;
private:
    GLuint index;
};
