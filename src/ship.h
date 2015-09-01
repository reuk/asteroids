#pragma once

#include "ship_graphic.h"
#include "space_object.h"
#include "windowed_app.h"
#include "bullet.h"
#include "bullet_graphic.h"
#include "key_timer.h"

#include <random>
#include <map>

struct ShipListener {
    virtual void ship_gun_fired(Bullet &&bullet) = 0;
};

class Ship : public SpaceObject, public WindowedApp::Listener, public ListenerList<ShipListener> {
public:
    Ship(ShipGraphic &ship_graphic, BulletGraphic &bullet_graphic);

    void update(float dt) override;

    void resize(const glm::vec2 &v) override;
    void error(const std::string &s) override;
    void key(int key, int scancode, int action, int mods) override;

private:
    glm::vec2 forward_vector() const;

    void forward(float dt);
    void backward(float dt);
    void left(float dt);
    void right(float dt);

    void fire();

    std::default_random_engine engine;
    std::uniform_real_distribution<float> angle_distribution;
    std::uniform_real_distribution<float> delta_distribution;

    StaticDrawable *bullet_graphic;

    std::map<int, std::pair<KeyTimer, decltype(&Ship::forward)>> key_timer;
};
