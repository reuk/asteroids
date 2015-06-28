#pragma once

#include "updatable.h"

template <typename T>
class Mover : public Updatable {
   public:
    Mover(const T& current = T(0), const T& delta = T(0))
        : current(current), delta(delta) {}

    void update() override { current += delta; }
    void impulse(const T& t) { delta += t; }

    void set_current(const T& t) { current = t; }
    T get_current() const { return current; }

    void set_delta(const T& t) { delta = t; }
    T get_delta() const { return delta; }

   private:
    T current;
    T delta;
};
