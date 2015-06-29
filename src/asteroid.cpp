#include "asteroid.h"

#include <random>
#include <algorithm>

using namespace std;
using namespace glm;

Asteroid::Asteroid(StaticDrawable& p, const Mover<vec2>& position,
                   const Mover<float>& angle, float size)
    : SpaceObject(p, size, position, angle) {}

vector<Asteroid> Asteroid::get_fragments() const {
    auto fragments = 3;

    vector<Asteroid> ret;

    auto next_size = size * 0.5;
    if (next_size < 0.02) return ret;

    for (auto i = 0; i != fragments; ++i) {
        default_random_engine engine{random_device()()};

        uniform_real_distribution<float> dir_dist(0, 2 * M_PI);
        uniform_real_distribution<float> speed_dist(0.0, 0.01);

        auto direction = dir_dist(engine);
        auto vel = position.get_delta() +
                   vec2(sin(direction), cos(direction)) * speed_dist(engine);

        auto ang = dir_dist(engine);
        auto del = speed_dist(engine);

        ret.emplace_back(
            move(Asteroid(*graphic, Mover<vec2>(position.get_current(), vel),
                          Mover<float>(ang, del), next_size)));
    }

    return ret;
}
