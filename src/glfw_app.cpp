#include "glfw_app.h"

#include <stdexcept>

using namespace std;

GLFWApp::GLFWApp() {
    if (!glfwInit()) {
        throw runtime_error("glfw failed to initialise");
    }
}

GLFWApp::~GLFWApp() {
    glfwTerminate();
}

GLFWerrorfun GLFWApp::set_error_callback(GLFWerrorfun a) {
    return glfwSetErrorCallback(a);
}

void GLFWApp::window_hint(int a, int b) {
    glfwWindowHint(a, b);
}

void GLFWApp::swap_interval(int a) {
    glfwSwapInterval(a);
}

double GLFWApp::get_time() {
    return glfwGetTime();
}

void GLFWApp::poll_events() {
    glfwPollEvents();
}
