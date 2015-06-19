#pragma once

#include <glm/glm.hpp>

#include "windowed_app.h"
#include "vao.h"
#include "buffer_object.h"
#include "program.h"

#include <string>

class Mover: public WindowedApp::Listener {
public:
    Mover(Program & p);

    void update();
    void draw() const;

    glm::vec2 get_position() const;
    glm::vec2 get_velocity() const;
    float get_angle() const;

    void resize(const glm::vec2 & v) override;
    void error(const std::string & s) override;
    void key(int key, int scancode, int action, int mods) override;
private:
    glm::vec2 forward() const;
    glm::vec2 backward() const;
    float left() const;
    float right() const;

    Program & shader_program;

    VAO vao;
    VBO geometry;
    VBO colors;
    IBO ibo;

    glm::vec2 position;
    glm::vec2 velocity;
    float angle;
    float dangle;
    float aspect;

    glm::mat4 mvp;
};
