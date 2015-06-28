#pragma once

#include <memory>

#include <GLFW/glfw3.h>

class Window {
   private:
    struct Destructor {
        inline void operator()(GLFWwindow *w) const;
    };

   public:
    Window();
    Window(int w, int h, const char *title, GLFWmonitor *m, GLFWwindow *s);

    virtual ~Window();

    Window(Window &&rhs);
    Window &operator=(Window &&rhs);

    Window(const Window &rhs) = delete;
    Window &operator=(const Window &rhs) = delete;

    static void default_hints();
    static void hint(int, int);
    int get_should_close() const;
    void set_should_close(int);
    void set_title(const char *);
    void get_pos(int &, int &) const;
    void set_pos(int, int);
    void get_size(int &, int &) const;
    void set_size(int, int);
    void get_framebuffer_size(int &, int &) const;
    // void get_frame_size(int &, int &, int &, int &) const;
    void iconify();
    void restore();
    void show();
    void hide();
    GLFWmonitor *get_monitor() const;
    int get_attrib(int) const;
    void set_user_pointer(void *);
    void *get_user_pointer() const;
    GLFWwindowposfun set_pos_callback(GLFWwindowposfun);
    GLFWwindowsizefun set_size_callback(GLFWwindowsizefun);
    GLFWwindowclosefun set_close_callback(GLFWwindowclosefun);
    GLFWwindowrefreshfun set_refresh_callback(GLFWwindowrefreshfun);
    GLFWwindowfocusfun set_focus_callback(GLFWwindowfocusfun);
    GLFWwindowiconifyfun set_iconify_callback(GLFWwindowiconifyfun);
    GLFWframebuffersizefun set_framebuffer_size_callback(
        GLFWframebuffersizefun);
    GLFWkeyfun set_key_callback(GLFWkeyfun);
    void swap_buffers();

    void make_context_current();

    std::unique_ptr<GLFWwindow, Destructor> window;
};
