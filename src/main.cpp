#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logger.h"
#include "windowed_app.h"
#include "engine.h"

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
        previous_seconds(glfwGetTime()),
        frame_count(0)
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
        mover.draw();
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

    double previous_seconds;
    unsigned int frame_count;

    Mover mover;
};

const string Asteroids::name = "asteroids";

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
