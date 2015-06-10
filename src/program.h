#pragma once

#include <GLFW/glfw3.h>

#include "shader.h"

#include <string>

class Program {
public:
    Program();
    virtual ~Program();

    void attach(const Shader & s) const;
    void detach(const Shader & s) const;
    void link() const;
    void use() const;

    GLint get_attrib_location(const std::string & name) const;

    bool verify() const;

    GLuint get_index() const;
private:
    GLuint index;
};
