#pragma once

#include <GLFW/glfw3.h>

#include "updatable.h"
#include "drawable.h"

class GLFWApp : public Updatable, public Drawable {
public:
    GLFWApp();
    virtual ~GLFWApp();

    static GLFWerrorfun set_error_callback(GLFWerrorfun);
    static void window_hint(int, int);
    static void swap_interval(int);
    static double get_time();
    static void poll_events();
};
