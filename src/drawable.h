#pragma once

#include "vao.h"
#include "buffer_object.h"
#include "generic_shader.h"

#include <vector>

class Drawable {
public:
    virtual void draw() const = 0;
};

class StaticDrawable: public Drawable {
public:
    StaticDrawable(
            GenericShader & shader_program,
            const std::vector<GLfloat> & geometry,
            const std::vector<GLfloat> & colors,
            const std::vector<GLushort> & indices);

    StaticDrawable(const StaticDrawable & rhs) noexcept;
    StaticDrawable & operator = (const StaticDrawable & rhs) noexcept;
    StaticDrawable(StaticDrawable && rhs) noexcept;
    StaticDrawable & operator = (StaticDrawable && rhs) noexcept;

    void draw() const override;

    GenericShader & shader_program;

private:
    void configure_vao() const;

    VAO vao;
    VBO geometry;
    VBO colors;
    IBO ibo;

    int size;
};
