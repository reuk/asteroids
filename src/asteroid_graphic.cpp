#include "asteroid_graphic.h"
#include "geometry_helpers.h"

#include <random>
#include <algorithm>

using namespace std;
using namespace glm;

vector<vec3> noise_circle(int points) {
    default_random_engine engine{random_device()()};
    normal_distribution<float> noise_dist(1.0, 0.1);

    auto counter = indices(points);

    vector<float> angles(points);
    transform(begin(counter), end(counter), begin(angles),
              [points](auto i) { return (i++ * 2 * M_PI) / points; });

    vector<vec3> ret(points);
    transform(begin(angles), end(angles), begin(ret),
              [&engine, &noise_dist](auto i) {
                  auto radius = noise_dist(engine);
                  return vec3(sin(i), cos(i), 0) * radius;
              });
    return ret;
}

AsteroidGraphic::AsteroidGraphic(GenericShader &shader_program)
    : StaticDrawable(shader_program, format(noise_circle(POINTS)),
                     format(vector<vec3>(POINTS, vec3(0, 1, 1))),
                     indices(POINTS)) {
}
