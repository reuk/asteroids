#pragma once

#include <glm/glm.hpp>

#include "windowed_app.h"
#include "vao.h"
#include "buffer_object.h"
#include "program.h"
#include "updatable.h"
#include "drawable.h"

#include <string>

template<typename T>
class Mover: public Updatable {
public:
    Mover():
        current(0),
        delta(0)
    {}

    virtual ~Mover() = default;

    void update() override { current += delta; }
    void impulse(const T & t) { delta += t; }

    void set_current(const T & t) { current = t; }
    T get_current() const { return current; }
private:
    T current;
    T delta;
};

class Ship: public WindowedApp::Listener, public Updatable, public StaticDrawable {
public:
    Ship(Program & p);
    virtual ~Ship();

    void update() override;
    void draw() const override;

    void resize(const glm::vec2 & v) override;
    void error(const std::string & s) override;
    void key(int key, int scancode, int action, int mods) override;
private:
    glm::vec2 forward() const;
    glm::vec2 backward() const;
    float left() const;
    float right() const;

    Program & shader_program;

    Mover<glm::vec2> position;
    Mover<float> angle;

    glm::vec4 viewport;
    float aspect;
};
