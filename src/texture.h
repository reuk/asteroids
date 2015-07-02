#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

class Texture {
public:
    Texture();
    virtual ~Texture();

    void bind() const;
    static void unbind();
    void data(GLsizei w, GLsizei h, GLubyte * t) const;
    GLuint get_index() const;

private:
    GLuint index;
};
