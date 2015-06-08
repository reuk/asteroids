#pragma once

#include "vec.h"

class Mover {
public:
    Mover();

    virtual void update() = 0;

    Vecf2 get_position() const;
    Vecf2 get_velocity() const;
    float get_angle() const;
private:
    Vecf2 position;
    Vecf2 velocity;
    float angle;
};
