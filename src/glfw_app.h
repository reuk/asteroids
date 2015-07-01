#pragma once

#include <GLFW/glfw3.h>

class GLFWApp {
public:
    GLFWApp();
    virtual ~GLFWApp();

    static GLFWerrorfun set_error_callback(GLFWerrorfun);
    static void window_hint(int, int);
    static void swap_interval(int);
    static double get_time();
    static void poll_events();

    virtual void update() = 0;
    virtual void draw() const = 0;
};
