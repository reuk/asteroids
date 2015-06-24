#pragma once

#include "vao.h"
#include "buffer_object.h"
#include "shader_program.h"

#include <vector>

class Drawable {
public:
    virtual void draw() const = 0;
};

class StaticDrawable: public Drawable {
public:
    StaticDrawable(
            Program & shader_program,
            const std::vector<GLfloat> & geometry,
            const std::vector<GLfloat> & colors,
            const std::vector<GLushort> & indices);

    StaticDrawable(const StaticDrawable & rhs) = delete;
    StaticDrawable & operator = (const StaticDrawable & rhs) = delete;
    StaticDrawable(StaticDrawable && rhs) noexcept;
    StaticDrawable & operator = (StaticDrawable && rhs) = delete;

    void draw() const override;

private:
    void configure_vao() const;

    Program & shader_program;

    VAO vao;
    VBO geometry;
    VBO colors;
    IBO ibo;

    int size;
};
