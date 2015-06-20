#pragma once

#include "vao.h"
#include "buffer_object.h"
#include "shader_program.h"

#include <vector>

class Drawable {
public:
    virtual ~Drawable() {}

    virtual void draw() const = 0;
};

class StaticDrawable: public Drawable {
public:
    StaticDrawable(
            Program & shader_program,
            const std::vector<GLfloat> & geometry,
            const std::vector<GLfloat> & colors,
            const std::vector<GLushort> & indices);

    void draw() const override;

private:
    VAO vao;
    VBO geometry;
    VBO colors;
    IBO ibo;

    int size;
};
