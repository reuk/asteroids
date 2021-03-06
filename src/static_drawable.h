#pragma once

#include "drawable.h"
#include "vao.h"
#include "buffer_object.h"
#include "generic_shader.h"

#include <glm/glm.hpp>

#include <vector>

class StaticDrawable : public Drawable {
public:
    StaticDrawable(GenericShader &shader_program,
                   const std::vector<GLfloat> &geometry,
                   const std::vector<GLfloat> &colors,
                   const std::vector<GLushort> &indices);

    StaticDrawable(const StaticDrawable &rhs) noexcept = delete;
    StaticDrawable &operator=(const StaticDrawable &rhs) noexcept = delete;
    StaticDrawable(StaticDrawable &&rhs) noexcept;
    StaticDrawable &operator=(StaticDrawable &&rhs) noexcept;

    void set_model_matrix(const glm::mat4 &model_matrix) const;

    void draw() const override;

    GenericShader &shader_program;

private:
    void configure_vao() const;

    VAO vao;
    StaticVBO geometry;
    StaticVBO colors;
    StaticIBO ibo;

    int size;
};
