#include "window.h"

#include <new>

using namespace std;

void Window::Destructor::operator()(GLFWwindow *w) const {
    glfwDestroyWindow(w);
}

Window::Window() {
}

Window::Window(int w, int h, const char *t, GLFWmonitor *m, GLFWwindow *s)
    : window(glfwCreateWindow(w, h, t, m, s)) {
    if (window == nullptr) {
        throw bad_alloc();
    }
}

Window::~Window() = default;

Window::Window(Window &&rhs) = default;
Window &Window::operator=(Window &&rhs) = default;

void Window::default_hints() {
    glfwDefaultWindowHints();
}

void Window::hint(int a, int b) {
    glfwWindowHint(a, b);
}

int Window::get_should_close() const {
    return glfwWindowShouldClose(window.get());
}

void Window::set_should_close(int a) {
    glfwSetWindowShouldClose(window.get(), a);
}

void Window::set_title(const char *a) {
    glfwSetWindowTitle(window.get(), a);
}

void Window::get_pos(int &a, int &b) const {
    glfwGetWindowPos(window.get(), &a, &b);
}

void Window::set_pos(int a, int b) {
    glfwSetWindowPos(window.get(), a, b);
}

void Window::get_size(int &a, int &b) const {
    glfwGetWindowSize(window.get(), &a, &b);
}

void Window::set_size(int a, int b) {
    glfwSetWindowSize(window.get(), a, b);
}

void Window::get_framebuffer_size(int &a, int &b) const {
    glfwGetFramebufferSize(window.get(), &a, &b);
}

// void Window::get_frame_size(int & a, int & b, int & c, int & d) const {
//    glfwGetWindowFrameSize(window.get(), &a, &b, &c, &d);
//}

void Window::iconify() {
    glfwIconifyWindow(window.get());
}

void Window::restore() {
    glfwRestoreWindow(window.get());
}

void Window::show() {
    glfwShowWindow(window.get());
}

void Window::hide() {
    glfwHideWindow(window.get());
}

GLFWmonitor *Window::get_monitor() const {
    return glfwGetWindowMonitor(window.get());
}

int Window::get_attrib(int a) const {
    return glfwGetWindowAttrib(window.get(), a);
}

void Window::set_user_pointer(void *a) {
    glfwSetWindowUserPointer(window.get(), a);
}

void *Window::get_user_pointer() const {
    return glfwGetWindowUserPointer(window.get());
}

GLFWwindowposfun Window::set_pos_callback(GLFWwindowposfun a) {
    return glfwSetWindowPosCallback(window.get(), a);
}

GLFWwindowsizefun Window::set_size_callback(GLFWwindowsizefun a) {
    return glfwSetWindowSizeCallback(window.get(), a);
}

GLFWwindowclosefun Window::set_close_callback(GLFWwindowclosefun a) {
    return glfwSetWindowCloseCallback(window.get(), a);
}

GLFWwindowrefreshfun Window::set_refresh_callback(GLFWwindowrefreshfun a) {
    return glfwSetWindowRefreshCallback(window.get(), a);
}

GLFWwindowfocusfun Window::set_focus_callback(GLFWwindowfocusfun a) {
    return glfwSetWindowFocusCallback(window.get(), a);
}

GLFWwindowiconifyfun Window::set_iconify_callback(GLFWwindowiconifyfun a) {
    return glfwSetWindowIconifyCallback(window.get(), a);
}

GLFWframebuffersizefun Window::set_framebuffer_size_callback(
    GLFWframebuffersizefun a) {
    return glfwSetFramebufferSizeCallback(window.get(), a);
}

GLFWkeyfun Window::set_key_callback(GLFWkeyfun a) {
    return glfwSetKeyCallback(window.get(), a);
}

void Window::swap_buffers() {
    glfwSwapBuffers(window.get());
}

void Window::make_context_current() {
    glfwMakeContextCurrent(window.get());
}
