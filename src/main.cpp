#include "logger.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <string>

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

Logger logger;

void error_callback(int error, const char * description) {
    logger.log_err("error ", error, ": ", description);
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}

struct __Window {
    template<typename T>
    inline void operator()(T && t) const {glfwDestroyWindow(t);}
};
using Window = unique_ptr<GLFWwindow, __Window>;

int main() {
    logger.restart();
    logger.log_err("starting...");
    logger.log_err("GLFW version: ", glfwGetVersionString());

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
        Window window(glfwCreateWindow(512, 512, "asteroids", nullptr, nullptr));
        if (window == nullptr) {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(window.get());
        glfwSwapInterval(1);

        glfwSetKeyCallback(window.get(), key_callback);

        glewExperimental = GL_TRUE;
        glewInit();

        logger.log_err("Renderer: ", glGetString(GL_RENDERER));
        logger.log_err("OpenGL version: ", glGetString(GL_VERSION));

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

        while (!glfwWindowShouldClose(window.get())) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(shader_program);
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            glfwSwapBuffers(window.get());
            glfwPollEvents();
        }
    }

    glfwTerminate();
}
