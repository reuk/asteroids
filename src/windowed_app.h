#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "window.h"
#include "glfw_app.h"
#include "listener_list.h"

#include <string>

class WindowedApp : public GLFWApp {
public:
  class Listener {
  public:
    virtual void resize(const glm::vec2 &v) = 0;
    virtual void error(const std::string &s) = 0;
    virtual void key(int key, int scancode, int action, int mods) = 0;
  };

  WindowedApp();

  void run();

  static void resize_callback(GLFWwindow *window, int w, int h);
  static void error_callback(int error, const char *description);
  static void key_callback(GLFWwindow *window, int key, int scancode,
                           int action, int mods);

  glm::vec2 get_size() const;
  Window &get_window();

  static void add_listener(Listener *l);
  static void remove_listener(Listener *l);

private:
  static glm::vec2 size;
  static ListenerList<Listener> listener_list;
  Window window;
};
