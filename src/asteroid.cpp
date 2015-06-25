#include "asteroid.h"

#include <random>

using namespace std;
using namespace glm;

vector<GLushort> indices(int points) {
    vector<GLushort> ret(points);
    auto i = 0;
    generate(begin(ret), end(ret), [&i]() { return i++; });
    return ret;
}

vector<vec3> noise_circle(int points) {
    default_random_engine engine{random_device()()};
    normal_distribution<float> noise_dist(1.0, 0.1);

    auto counter = indices(points);

    vector<float> angles(points);
    transform(
            begin(counter),
            end(counter),
            begin(angles),
            [points](auto i) {
                return (i++ * 2 * M_PI) / points;
            });

    vector<vec3> ret(points);
    transform(
            begin(angles),
            end(angles),
            begin(ret),
            [&engine, &noise_dist](auto i) {
                auto radius = noise_dist(engine);
                return vec3(sin(i), cos(i), 0) * radius;
            });
    return ret;
}

vector<GLfloat> format(const vector<vec3> & points) {
    vector<GLfloat> ret(points.size() * 3);
    for (auto i = 0; i != points.size(); ++i) {
        auto ret_index = i * 3;
        ret[ret_index + 0] = points[i].x;
        ret[ret_index + 1] = points[i].y;
        ret[ret_index + 2] = points[i].z;
    }
    return ret;
}

Asteroid::Asteroid(
        Program & shader_program,
        const Mover<vec2> & position,
        const Mover<float> & angle,
        float size):
    SpaceObject(
            shader_program,
            format(noise_circle(POINTS)),
            format(vector<vec3>(POINTS, vec3(0, 1, 1))),
            indices(POINTS),
            size,
            position,
            angle),
    size(size)
{

}

vector<Asteroid> Asteroid::get_fragments() const {
    auto fragments = 4;

    vector<Asteroid> ret;

    auto next_size = size * 0.5;
    if (next_size < 0.02)
        return ret;

    for (auto i = 0; i != fragments; ++i) {
        default_random_engine engine{random_device()()};

        uniform_real_distribution<float> dir_dist(0, 2 * M_PI);
        uniform_real_distribution<float> speed_dist(0.0, 0.01);

        auto direction = dir_dist(engine);
        auto vel = vec2(sin(direction), cos(direction)) * speed_dist(engine);

        auto ang = dir_dist(engine);
        auto del = speed_dist(engine);

        ret.emplace_back(move(Asteroid(
                        shader_program,
                        Mover<vec2>(position.get_current(), vel),
                        Mover<float>(ang, del),
                        next_size)));
    }

    return ret;
}
