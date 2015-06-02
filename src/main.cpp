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
    vector<pair<GLenum, string>> int_params = {
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

    vector<pair<GLenum, string>> two_int_params = {
        {GL_MAX_VIEWPORT_DIMS                   , "GL_MAX_VIEWPORT_DIMS"},
    };

    vector<pair<GLenum, string>> bool_params = {
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

template<typename T>
void print_info_log(GLuint shader, T func) {
    char log[2048];
    auto length = 0;
    func(shader, sizeof(log), &length, log);
    if (length) {
        Logger::log("info log for index ", shader, ":");
        Logger::log(log);
    }
}

template<typename T, typename U>
auto check_gl(GLuint item, GLenum flag, T a, U b) {
    auto params = -1;
    a(item, flag, &params);
    if (params != GL_TRUE) {
        Logger::log_err("ERROR: GL index ", item);
        print_info_log(item, b);
        return false;
    }
    return true;
}

auto check_shader(GLuint item) {
    return check_gl(item, GL_COMPILE_STATUS, glGetShaderiv, glGetShaderInfoLog);
}

auto check_program(GLuint item) {
    return check_gl(item, GL_LINK_STATUS, glGetProgramiv, glGetProgramInfoLog);
}

template<typename T, typename U>
void print_params(GLuint item, int params, T a, U b) {
    const map<GLenum, string> type_strings = {
        {GL_BOOL                    , "GL_BOOL"},
        {GL_INT                     , "GL_INT"},
        {GL_FLOAT                   , "GL_FLOAT"},
        {GL_FLOAT_VEC2              , "GL_FLOAT_VEC2"},
        {GL_FLOAT_VEC3              , "GL_FLOAT_VEC3"},
        {GL_FLOAT_VEC4              , "GL_FLOAT_VEC4"},
        {GL_FLOAT_MAT2              , "GL_FLOAT_MAT2"},
        {GL_FLOAT_MAT3              , "GL_FLOAT_MAT3"},
        {GL_FLOAT_MAT4              , "GL_FLOAT_MAT4"},
        {GL_SAMPLER_2D              , "GL_SAMPLER_2D"},
        {GL_SAMPLER_3D              , "GL_SAMPLER_3D"},
        {GL_SAMPLER_CUBE            , "GL_SAMPLER_CUBE"},
        {GL_SAMPLER_2D_SHADOW       , "GL_SAMPLER_2D_SHADOW"},
    };

    for (auto i = 0; i != params; ++i) {
        char name[64];
        auto length = 0;
        auto size = 0;
        GLenum type;
        a(item, i, sizeof(name), &length, &size, &type, name);

        if (size > 1) {
            for (auto j = 0; j != size; ++j) {
                stringstream ss;
                ss << name << "[" << j << "]";
                int location = b(item, ss.str().c_str());
                Logger::log(i, " type: ", type_strings.at(type), " name: ", ss.str(), " location: ", location);
            }
        } else {
            int location = b(item, name);
            Logger::log(i, " type: ", type_strings.at(type), " name: ", name, " location: ", location);
        }
    }
}

void simple_print(GLuint item, const pair<GLenum, string> & i, int & params) {
    glGetProgramiv(item, i.first, &params);
    Logger::log(i.second, ": ", params);
}

void print_all(GLuint item) {
    Logger::log("--- shader program ", item, " info ---");
    auto params = -1;

    simple_print(item, {GL_LINK_STATUS         , "GL_LINK_STATUS"},         params);
    simple_print(item, {GL_ATTACHED_SHADERS    , "GL_ATTACHED_SHADERS"},    params);

    simple_print(item, {GL_ACTIVE_ATTRIBUTES   , "GL_ACTIVE_ATTRIBUTES"},   params);
    print_params(item, params, glGetActiveAttrib, glGetAttribLocation);

    simple_print(item, {GL_ACTIVE_UNIFORMS     , "GL_ACTIVE_UNIFORMS"},     params);
    print_params(item, params, glGetActiveUniform, glGetUniformLocation);

    print_info_log(item, glGetProgramInfoLog);
}

auto verify(GLuint item) {
    glValidateProgram(item);

    auto params = -1;
    simple_print(item, {GL_VALIDATE_STATUS, "GL_VALIDATE_STATUS"}, params);

    if (params != GL_TRUE) {
        print_info_log(item, glGetProgramInfoLog);
        return false;
    }
    return true;
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
        check_shader(fs);

        GLuint vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vertex_shader_ptr, nullptr);
        glCompileShader(vs);
        check_shader(vs);

        GLuint shader_program = glCreateProgram();
        glAttachShader(shader_program, fs);
        glAttachShader(shader_program, vs);
        glLinkProgram(shader_program);

        if (! verify(shader_program)) {
            glfwTerminate();
            exit(EXIT_FAILURE);
        }
        check_program(shader_program);

        print_all(shader_program);

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
