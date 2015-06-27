#pragma once

#include "space_object.h"
#include "windowed_app.h"
#include "bullet.h"

#include <random>

class Ship: public SpaceObject, public WindowedApp::Listener {
public:
    class Listener {
    public:
        virtual void ship_gun_fired(Bullet && bullet) = 0;
    };

    Ship(GenericShader & p);

    void resize(const glm::vec2 & v) override;
    void error(const std::string & s) override;
    void key(int key, int scancode, int action, int mods) override;

    void add_listener(Listener * listener);
    void remove_listener(Listener * listener);

    void set_life(float life);
    float get_life() const;

private:
    glm::vec2 forward_vector() const;

    glm::vec2 forward() const;
    glm::vec2 backward() const;
    float left() const;
    float right() const;
    void fire();

    float life;

    ListenerList<Listener> listener_list;

    std::default_random_engine engine;
    std::uniform_real_distribution<float> angle_distribution;
    std::uniform_real_distribution<float> delta_distribution;
};

