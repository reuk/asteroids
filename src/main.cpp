#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logger.h"
#include "windowed_app.h"
#include "asteroid.h"
#include "ship.h"
#include "bullet.h"
#include "screen_boundary.h"
#include "shader_program.h"
#include "key_callback.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdexcept>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <unordered_set>

using namespace std;
using namespace glm;

class Asteroids: public WindowedApp, public WindowedApp::Listener, public Ship::Listener {
public:
    Asteroids():
        shader_program(vertex_shader, fragment_shader),
#ifdef DEBUG
        previous_seconds(glfwGetTime()),
        frame_count(0),
#endif
        screen_boundary(shader_program),
        ship(shader_program)
    {
        get_window().set_title(name.c_str());

        Logger::log("GLFW version: ", glfwGetVersionString());
        Logger::log("Renderer: ", glGetString(GL_RENDERER));
        Logger::log("OpenGL version: ", glGetString(GL_VERSION));

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        //  register listeners
        add_listener(this);
        add_listener(&ship);

        ship.add_listener(this);
    }

    template<typename T>
    void clear_marked(vector<T> & t, vector<bool> & marked) {
        auto i = t.begin();
        auto id = marked.begin();
        for (; i != t.end();) {
            if (*id) {
                i = t.erase(i);
                id = marked.erase(id);
            } else {
                ++i;
                ++id;
            }
        }
    }

    template<typename T>
    void split_marked(vector<T> & t, vector<bool> & marked) {
        vector<T> to_insert;

        auto i = t.begin();
        auto id = marked.begin();
        for (; i != t.end();) {
            if (*id) {
                auto fragments = i->get_fragments();
                to_insert.insert(to_insert.end(), fragments.begin(), fragments.end());
                i = t.erase(i);
                id = marked.erase(id);
            } else {
                ++i;
                ++id;
            }
        }
        t.insert(t.end(), to_insert.begin(), to_insert.end());
    }

    void update() override {
#ifdef DEBUG
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
#endif

        //  update each entity
        ship.update();
        for (auto && i : bullets)
            i.update();
        for (auto && i : asteroids)
            i.update();

        //  check for collisions
        for (auto && i : asteroids) {
            if (ship.is_hit(i)) {
                //  ship dies!
            }
        }

        vector<bool> asteroids_to_delete(asteroids.size(), false);
        vector<bool> bullets_to_delete(bullets.size(), false);

        auto i = asteroids.begin();
        auto id = asteroids_to_delete.begin();
        for (; i != asteroids.end(); ++i, ++id) {
            auto j = bullets.begin();
            auto jd = bullets_to_delete.begin();
            for (; j != bullets.end(); ++j, ++jd) {
                if (i->is_hit(*j)) {
                    *id = true;
                    *jd = true;
                }
            }
        }

        split_marked(asteroids, asteroids_to_delete);
        clear_marked(bullets, bullets_to_delete);
    }

    void draw() const override {
        shader_program.use();

        auto view_matrix = lookAt(vec3(0.0f, 0.0f, 2.0f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(shader_program.get_uniform_location("v_view"), 1, GL_FALSE, value_ptr(view_matrix));

        auto projection_matrix = perspective(45.0f, aspect, 0.1f, 10.0f);
        glUniformMatrix4fv(shader_program.get_uniform_location("v_projection"), 1, GL_FALSE, value_ptr(projection_matrix));

        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //screen_boundary.draw();
        ship.draw();
        for (const auto & i : bullets)
            i.draw();
        for (const auto & i : asteroids)
            i.draw();
        Program::unuse();
    }

    void resize(const vec2 & v) override {
        glViewport(0, 0, v.x, v.y);
        aspect = v.x / v.y;
    }

    void error(const std::string & s) override {
        Logger::log_err("error: ", s);
    }

    void add_asteroid() {
        default_random_engine engine{random_device()()};
        uniform_real_distribution<float> pos_dist(-1.0, 1.0);

        vec2 pos(pos_dist(engine), pos_dist(engine));

        uniform_real_distribution<float> dir_dist(0, 2 * M_PI);
        uniform_real_distribution<float> speed_dist(0.0, 0.01);

        auto direction = dir_dist(engine);
        auto vel = vec2(sin(direction), cos(direction)) * speed_dist(engine);

        auto ang = dir_dist(engine);
        auto del = speed_dist(engine);

        asteroids.emplace_back(move(Asteroid(
                        shader_program,
                        Mover<vec2>(pos, vel),
                        Mover<float>(ang, del))));

    }

    void split_all_asteroids() {
        vector<Asteroid> new_asteroids;
        for (auto && i : asteroids) {
            auto fragments = i.get_fragments();
            new_asteroids.insert(new_asteroids.end(), fragments.begin(), fragments.end());
        }
        asteroids = new_asteroids;
    }

    void key(int key, int scancode, int action, int mods) override {
        if (! (action == GLFW_PRESS || action == GLFW_REPEAT))
            return;

        key_dispatch<decltype(&Asteroids::add_asteroid)>(this, {
            {GLFW_KEY_N, &Asteroids::add_asteroid},
            {GLFW_KEY_M, &Asteroids::split_all_asteroids},
        }, key);
    }

    void ship_gun_fired(Bullet && bullet) override {
        bullets.emplace_back(move(bullet));
    }

private:
    static const string name;

    static const std::string vertex_shader;
    static const std::string fragment_shader;

    ShaderProgram shader_program;

#ifdef DEBUG
    double previous_seconds;
    unsigned int frame_count;
#endif

    float aspect;

    ScreenBoundary screen_boundary;
    Ship ship;
    vector<Bullet> bullets;
    vector<Asteroid> asteroids;
};

const string Asteroids::name = "asteroids";

const string Asteroids::vertex_shader(R"(
#version 330
in vec3 v_position;
in vec3 v_color;
out vec3 f_color;
uniform mat4 v_model;
uniform mat4 v_view;
uniform mat4 v_projection;
void main() {
    gl_Position = v_projection * v_view * v_model * vec4(v_position, 1.0);
    f_color = v_color;
}
)");

const string Asteroids::fragment_shader(R"(
#version 330
in vec3 f_color;
out vec4 frag_color;
void main() {
    frag_color = vec4(f_color, 1.0);
}
)");

int main() {
    Logger::restart();
    Logger::log(__FUNCTION__, " starting...");

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
