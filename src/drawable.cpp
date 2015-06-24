#include "drawable.h"

using namespace std;

StaticDrawable::StaticDrawable(
        Program & shader_program,
        const vector<GLfloat> & g,
        const vector<GLfloat> & c,
        const vector<GLushort> & i):
    shader_program(shader_program),
    size(i.size())
{
    geometry.data(g);
    colors.data(c);
    ibo.data(i);

    configure_vao();
}

StaticDrawable::StaticDrawable(StaticDrawable && rhs) noexcept:
    shader_program(rhs.shader_program),
    geometry(move(rhs.geometry)),
    colors(move(rhs.colors)),
    ibo(move(rhs.ibo)),
    size(rhs.size)
{
    configure_vao();
}

void StaticDrawable::draw() const {
    vao.bind();
    glDrawElements(GL_LINE_LOOP, size, GL_UNSIGNED_SHORT, nullptr);
    VAO::unbind();
}

void StaticDrawable::configure_vao() const {
    vao.bind();

    auto v_position = shader_program.get_attrib_location("v_position");
    glEnableVertexAttribArray(v_position);
    geometry.bind();
    glVertexAttribPointer(v_position, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    auto v_color = shader_program.get_attrib_location("v_color");
    glEnableVertexAttribArray(v_color);
    colors.bind();
    glVertexAttribPointer(v_color, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    ibo.bind();

    VAO::unbind();
}
