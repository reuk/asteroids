#pragma once

#include <glm/glm.hpp>

#include "windowed_app.h"
#include "vao.h"
#include "buffer_object.h"
#include "program.h"
#include "updatable.h"
#include "drawable.h"

#include <map>
#include <string>
#include <random>

template<typename T>
class Mover: public Updatable {
public:
    Mover(const T & current = T(0), const T & delta = T(0)):
        current(current),
        delta(delta)
    {}

    void update() override { current += delta; }
    void impulse(const T & t) { delta += t; }

    void set_current(const T & t) { current = t; }
    T get_current() const { return current; }

    void set_delta(const T & t) { delta = t; }
    T get_delta() const { return delta; }
private:
    T current;
    T delta;
};

class SpaceObject: public Updatable, public StaticDrawable {
public:
    SpaceObject(
            Program & shader_program,
            const std::vector<GLfloat> &,
            const std::vector<GLfloat> &,
            const std::vector<GLushort> &,
            float size,
            const Mover<glm::vec2> & position = Mover<glm::vec2>(),
            const Mover<float> & angle = Mover<float>());

    void draw() const override;
    void update() override;

    Program & shader_program;

    Mover<glm::vec2> position;
    Mover<float> angle;

    const float size;
};

class Bullet: public SpaceObject {
public:
    Bullet(
            Program & p,
            const Mover<glm::vec2> & position = Mover<glm::vec2>(),
            const Mover<float> & angle = Mover<float>());
};

class Ship: public SpaceObject, public WindowedApp::Listener {
public:
    class Listener {
    public:
        virtual void ship_gun_fired(Bullet && bullet) = 0;
    };

    Ship(Program & p);

    void resize(const glm::vec2 & v) override;
    void error(const std::string & s) override;
    void key(int key, int scancode, int action, int mods) override;

    void add_listener(Listener * listener);
    void remove_listener(Listener * listener);
private:
    glm::vec2 forward() const;
    glm::vec2 backward() const;
    float left() const;
    float right() const;
    void fire();

    ListenerList<Listener> listener_list;

    std::default_random_engine engine;
    std::uniform_real_distribution<float> angle_distribution;
    std::uniform_real_distribution<float> delta_distribution;
};

class Asteroid: public SpaceObject {
public:
    Asteroid(
            Program & p,
            const Mover<glm::vec2> & position = Mover<glm::vec2>(),
            const Mover<float> & angle = Mover<float>());
private:
    static const int POINTS = 10;
};
