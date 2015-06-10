#pragma once

#include <glm/glm.hpp>

#include "windowed_app.h"
#include "vao.h"
#include "buffer_object.h"
#include "program.h"

#include <string>

class Mover: public WindowedApp::Listener {
public:
    Mover();

    void update();
    void draw() const;

    glm::vec2 get_position() const;
    glm::vec2 get_velocity() const;
    float get_angle() const;

    void resize(const glm::vec2 & v) override;
    void error(const std::string & s) override;
    void key(int key, int scancode, int action, int mods) override;
private:
    VBO geometry;
    VBO colors;
    IBO ibo;
    VAO vao;

    static const std::string vertex_shader;
    static const std::string fragment_shader;

    Program shader_program;

    glm::vec2 position;
    glm::vec2 velocity;
    float angle;
};
