#include "logger.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

const string vertex_shader(R"(
#version 400
in vec3 vp;
void main() {
    gl_Position = vec4(vp, 1.0);
}
)");

const string fragment_shader(R"(
#version 400
out vec4 frag_color;
void main() {
    frag_color = vec4(0.5, 0.0, 0.5, 1.0);
}
)");

void error_callback(int error, const char * description) {
    Logger::log_err("error ", error, ": ", description);
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

auto win_w = 640;
auto win_h = 480;

void resize_callback(GLFWwindow * window, int w, int h) {
    win_w = w;
    win_h = h;
}

struct __Window {
    template<typename T>
    inline void operator()(T && t) const {glfwDestroyWindow(t);}
};
using Window = unique_ptr<GLFWwindow, __Window>;

void log_params() {
    map<GLenum, string> int_params = {
        {GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS    , "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS"},
        {GL_MAX_CUBE_MAP_TEXTURE_SIZE           , "GL_MAX_CUBE_MAP_TEXTURE_SIZE"},
        {GL_MAX_DRAW_BUFFERS                    , "GL_MAX_DRAW_BUFFERS"},
        {GL_MAX_FRAGMENT_UNIFORM_COMPONENTS     , "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS"},
        {GL_MAX_TEXTURE_IMAGE_UNITS             , "GL_MAX_TEXTURE_IMAGE_UNITS"},
        {GL_MAX_TEXTURE_SIZE                    , "GL_MAX_TEXTURE_SIZE"},
        {GL_MAX_VARYING_FLOATS                  , "GL_MAX_VARYING_FLOATS"},
        {GL_MAX_VERTEX_ATTRIBS                  , "GL_MAX_VERTEX_ATTRIBS"},
        {GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS      , "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS"},
        {GL_MAX_VERTEX_UNIFORM_COMPONENTS       , "GL_MAX_VERTEX_UNIFORM_COMPONENTS"},
    };

    map<GLenum, string> two_int_params = {
        {GL_MAX_VIEWPORT_DIMS                   , "GL_MAX_VIEWPORT_DIMS"},
    };

    map <GLenum, string> bool_params = {
        {GL_STEREO                              , "GL_STEREO"},
    };

    Logger::log("GL Context Parameters:");

    for (const auto & i : int_params) {
        GLint v = 0;
        glGetIntegerv(i.first, &v);
        Logger::log(i.second, ": ", v);
    }

    for (const auto & i : two_int_params) {
        GLint v[2] = {0, 0};
        glGetIntegerv(i.first, v);
        Logger::log(i.second, ": ", v[0], ", ", v[1]);
    }

    for (const auto & i : bool_params) {
        GLboolean v = 0;
        glGetBooleanv(i.first, &v);
        Logger::log(i.second, ": ", (bool)v);
    }

    Logger::log("--------------------------------");
}

int main() {
    Logger::restart();
    Logger::log_err("starting...");
    Logger::log_err("GLFW version: ", glfwGetVersionString());

    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    glfwWindowHint(GLFW_SAMPLES, 4);

    {
        auto & monitor = *glfwGetPrimaryMonitor();
        const auto & vmode = glfwGetVideoMode(&monitor);

        auto name = "asteroids";

        Window window(glfwCreateWindow(win_w, win_h, name, nullptr, nullptr));
        if (window == nullptr) {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window.get());
        glfwSwapInterval(1);

        glfwSetKeyCallback(window.get(), key_callback);
        glfwSetWindowSizeCallback(window.get(), resize_callback);

        glewExperimental = GL_TRUE;
        glewInit();

        Logger::log_err("Renderer: ", glGetString(GL_RENDERER));
        Logger::log_err("OpenGL version: ", glGetString(GL_VERSION));
        log_params();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        vector<float> points {0, 0.5, 0, 0.5, -0.5, 0, -0.5, -0.5, 0};

        GLuint vbo = 0;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(decltype(points)::value_type), points.data(), GL_STATIC_DRAW);

        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        auto vertex_shader_ptr      = vertex_shader.c_str();
        auto fragment_shader_ptr    = fragment_shader.c_str();

        GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fragment_shader_ptr, nullptr);
        glCompileShader(fs);

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertex_shader_ptr, nullptr);
        glCompileShader(vs);

        GLuint shader_program = glCreateProgram();
        glAttachShader(shader_program, fs);
        glAttachShader(shader_program, vs);
        glLinkProgram(shader_program);

        auto previous_seconds = glfwGetTime();
        auto frame_count = 0;

        //  main loop
        while (!glfwWindowShouldClose(window.get())) {
            auto current_seconds = glfwGetTime();
            auto elapsed_seconds = current_seconds - previous_seconds;
            if (elapsed_seconds > 0.1) {
                previous_seconds = current_seconds;
                auto fps = static_cast<decltype(elapsed_seconds)>(frame_count) / elapsed_seconds;

                stringstream ss;
                ss << name << " - fps: " << fps;

                glfwSetWindowTitle(window.get(), ss.str().c_str());
                frame_count = 0;
            }
            frame_count++;

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glViewport(0, 0, win_w, win_h);

            glUseProgram(shader_program);
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(window.get());
            glfwPollEvents();
        }
    }

    glfwTerminate();
}
