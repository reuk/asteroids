#pragma once

#include "windowed_app.h"

class KeyTimer {
public:
    double get_elapsed_time() const;
    void reset();

    void set_state(int s);
    int get_state() const;

private:
    double previous_seconds;
    int state;
};
