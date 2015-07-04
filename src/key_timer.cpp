#include "key_timer.h"

double KeyTimer::get_elapsed_time() const {
    auto current_seconds = glfwGetTime();
    return current_seconds - previous_seconds;
}

void KeyTimer::reset() {
    previous_seconds = glfwGetTime();
}

void KeyTimer::set_state(int s) {
    state = s;
}

int KeyTimer::get_state() const {
    return state;
}
