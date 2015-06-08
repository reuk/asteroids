#pragma once

#include <GLFW/glfw3.h>

#include "shader.h"

class Program {
public:
    Program();
    virtual ~Program();

    void attach(const Shader & s) const;
    void detach(const Shader & s) const;
    void link() const;
    void use() const;

    GLuint get_index() const;
private:
    GLuint index;
};
