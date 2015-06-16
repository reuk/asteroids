#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logger.h"
#include "windowed_app.h"
#include "engine.h"
#include "shader_program.h"

#include <stdexcept>
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;
using namespace glm;

class Asteroids: public WindowedApp, public WindowedApp::Listener {
public:
    Asteroids():
        shader_program(vertex_shader, fragment_shader),
        previous_seconds(glfwGetTime()),
        frame_count(0),
        mover(shader_program)
    {
        Logger::log_err("GLFW version: ", glfwGetVersionString());
        Logger::log_err("Renderer: ", glGetString(GL_RENDERER));
        Logger::log_err("OpenGL version: ", glGetString(GL_VERSION));

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        //  register listeners
        add_listener(this);
        add_listener(&mover);
    }

    void update() override {
        auto current_seconds = glfwGetTime();
        auto elapsed_seconds = current_seconds - previous_seconds;
        if (elapsed_seconds > 0.1) {
            previous_seconds = current_seconds;
            auto fps = (float)frame_count / elapsed_seconds;

            stringstream ss;
            ss << name << " - fps: " << fps;

            get_window().set_title(ss.str().c_str());
            frame_count = 0;
        }
        frame_count++;

        mover.update();
    }

    void draw() const override {
        shader_program.use();
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mover.draw();
        Program::unuse();
    }

    void resize(const vec2 & v) override {
        glViewport(0, 0, v.x, v.y);
    }

    void error(const std::string & s) override {
        Logger::log_err("error: ", s);
    }

    void key(int key, int scancode, int action, int mods) override {

    }

private:
    static const string name;

    static const std::string vertex_shader;
    static const std::string fragment_shader;

    ShaderProgram shader_program;

    double previous_seconds;
    unsigned int frame_count;

    Mover mover;
};

const string Asteroids::name = "asteroids";

const string Asteroids::vertex_shader(R"(
#version 400
in vec3 v_position;
in vec3 v_color;
out vec3 f_color;
uniform mat4 v_mvp;
void main() {
    gl_Position = v_mvp * vec4(v_position, 1.0);
    f_color = v_color;
}
)");

const string Asteroids::fragment_shader(R"(
#version 400
in vec3 f_color;
out vec4 frag_color;
void main() {
    frag_color = vec4(f_color, 1.0);
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
