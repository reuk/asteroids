#include "windowed_app.h"

#include "logger.h"

using namespace std;
using namespace glm;

WindowedApp::WindowedApp() {
    set_error_callback(error_callback);
    window_hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    window_hint(GLFW_CONTEXT_VERSION_MINOR, 2);
    window_hint(GLFW_OPENGL_FORWARD_COMPAT, true);
    window_hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window_hint(GLFW_SAMPLES, 4);
    window_hint(GLFW_RESIZABLE, false);

    window = Window(size.x, size.y, "", nullptr, nullptr);

    window.make_context_current();
    glfwSwapInterval(1);

    glewExperimental = GL_TRUE;
    glewInit();

    window.set_key_callback(key_callback);
    window.set_size_callback(resize_callback);

    Logger::log("created window");
}

void WindowedApp::run() {
    auto w = 0;
    auto h = 0;
    window.get_framebuffer_size(w, h);
    listener_list.call(&Listener::resize, vec2(w, h));

    while (! window.get_should_close()) {
        update();
        draw();

        window.swap_buffers();
        poll_events();
    }
}

void WindowedApp::resize_callback(GLFWwindow * window, int w, int h) {
    size = vec2(w, h);
    listener_list.call(&Listener::resize, size);
}

void WindowedApp::error_callback(int error, const char * description) {
    Logger::log_err("error ", error, ": ", description);
    listener_list.call(&Listener::error, description);
}

void WindowedApp::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    listener_list.call(&Listener::key, key, scancode, action, mods);
}

vec2 WindowedApp::get_size() const {
    return size;
}

Window & WindowedApp::get_window() {
    return window;
}

void WindowedApp::add_listener(Listener * l) {
    listener_list.add(l);
}

void WindowedApp::remove_listener(Listener * l) {
    listener_list.remove(l);
}

vec2 WindowedApp::size(500, 500);
ListenerList<WindowedApp::Listener> WindowedApp::listener_list;
