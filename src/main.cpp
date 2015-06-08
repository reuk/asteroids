#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logger.h"
#include "window.h"
#include "glfw_app.h"
#include "program.h"
#include "vao.h"
#include "vbo.h"
#include "engine.h"

#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

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
    vector<char> log(2048);
    auto length = 0;
    func(shader, log.size() * sizeof(decltype(log)::value_type), &length, log.data());
    if (length) {
        Logger::log("info log for index ", shader, ":");
        Logger::log(log.data());
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
        vector<char> name(64);
        auto length = 0;
        auto size = 0;
        GLenum type;
        a(item, i, name.size() * sizeof(decltype(name)::value_type), &length, &size, &type, name.data());

        if (size > 1) {
            for (auto j = 0; j != size; ++j) {
                stringstream ss;
                ss << name.data() << "[" << j << "]";
                int location = b(item, ss.str().c_str());
                Logger::log(i, " type: ", type_strings.at(type), " name: ", ss.str(), " location: ", location);
            }
        } else {
            int location = b(item, name.data());
            Logger::log(i, " type: ", type_strings.at(type), " name: ", name.data(), " location: ", location);
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

class WindowedApp: public GLFWApp {
public:
    WindowedApp() {
        set_error_callback(error_callback);
#ifdef __APPLE__
        window_hint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        window_hint(GLFW_CONTEXT_VERSION_MINOR, 2);
        window_hint(GLFW_OPENGL_FORWARD_COMPAT, true);
        window_hint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
        window_hint(GLFW_SAMPLES, 4);

        window = Window(win_w, win_h, "", nullptr, nullptr);

        window.make_context_current();
        glfwSwapInterval(1);

        glewExperimental = GL_TRUE;
        glewInit();

        window.set_key_callback(key_callback);
        window.set_size_callback(resize_callback);

        Logger::log_err("created window");
    }

    void run() {
        while (! window.get_should_close()) {
            update();
            draw();

            window.swap_buffers();
            poll_events();
        }
    }

    static void resize_callback(GLFWwindow * window, int w, int h) {
        win_w = w;
        win_h = h;
    }

    static void error_callback(int error, const char * description) {
        Logger::log_err("error ", error, ": ", description);
    }

    static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }
    }

    int get_width() const {
        return win_w;
    }

    int get_height() const {
        return win_h;
    }

    Window & get_window() {
        return window;
    }

private:
    static int win_w;
    static int win_h;

    Window window;
};

int WindowedApp::win_w = 640;
int WindowedApp::win_h = 480;

class Asteroids: public WindowedApp {
public:
    Asteroids():
        previous_seconds(glfwGetTime()),
        frame_count(0)
    {
        Logger::log_err("GLFW version: ", glfwGetVersionString());
        Logger::log_err("Renderer: ", glGetString(GL_RENDERER));
        Logger::log_err("OpenGL version: ", glGetString(GL_VERSION));
        log_params();

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        //  load geometry
        vbo.bind();
        vbo.data(vector<float>{0.0f, 0.5f, 0.0f, 0.5f, -0.5f, 0.0f, -0.5f, -0.5f, 0.0f}, GL_STATIC_DRAW);

        vao.bind();
        glEnableVertexAttribArray(0);
        vbo.bind();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        //  load shaders
        Shader fs(GL_FRAGMENT_SHADER);
        fs.source(fragment_shader);
        fs.compile();

        check_shader(fs.get_index());

        Shader vs(GL_VERTEX_SHADER);
        vs.source(vertex_shader);
        vs.compile();

        check_shader(vs.get_index());

        shader_program.attach(fs);
        shader_program.attach(vs);
        shader_program.link();

        if (! verify(shader_program.get_index())) {
            throw runtime_error("shader program failed to verify");
        }
        check_program(shader_program.get_index());

        print_all(shader_program.get_index());
    }

    void update() override {
        auto current_seconds = glfwGetTime();
        auto elapsed_seconds = current_seconds - previous_seconds;
        if (elapsed_seconds > 0.1) {
            previous_seconds = current_seconds;
            auto fps = static_cast<decltype(elapsed_seconds)>(frame_count) / elapsed_seconds;

            stringstream ss;
            ss << name << " - fps: " << fps;

            get_window().set_title(ss.str().c_str());
            frame_count = 0;
        }
        frame_count++;
    }

    void draw() const override {
        glClearColor(0.1, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, get_width(), get_height());

        shader_program.use();
        vao.bind();
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

private:
    static const string name;
    static const string vertex_shader;
    static const string fragment_shader;

    VBO vbo;
    VAO vao;
    Program shader_program;

    double previous_seconds;
    unsigned int frame_count;
};

const string Asteroids::name = "asteroids";

const string Asteroids::vertex_shader(R"(
#version 400
in vec3 vp;
void main() {
    gl_Position = vec4(vp, 1.0);
}
)");

const string Asteroids::fragment_shader(R"(
#version 400
out vec4 frag_color;
void main() {
    frag_color = vec4(0.0, 1.0, 1.0, 1.0);
}
)");


int main() {
    Logger::restart();
    Logger::log_err("starting...");

    try {
        Asteroids asteroids;
        asteroids.run();
    } catch (const runtime_error & re) {
        Logger::log_err(re.what());
        return 1;
    } catch (const bad_alloc & ba) {
        Logger::log_err(ba.what());
        return 1;
    } catch (...) {
        Logger::log_err("program encountered unknown error");
        return 1;
    }
}
