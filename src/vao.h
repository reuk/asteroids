#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VAO {
   public:
    VAO();
    virtual ~VAO();

    void bind() const;
    static void unbind();

    GLuint get_index() const;

   private:
    GLuint index;
};
