#include "geometry_helpers.h"

#include <algorithm>

using namespace std;
using namespace glm;

vector<GLushort> indices(int points) {
    vector<GLushort> ret(points);
    auto i = 0;
    generate(begin(ret), end(ret), [&i]() { return i++; });
    return ret;
}

vector<GLfloat> format(const vector<vec3> &points) {
    vector<GLfloat> ret(points.size() * 3);
    for (auto i = 0u; i != points.size(); ++i) {
        auto ret_index = i * 3;
        ret[ret_index + 0] = points[i].x;
        ret[ret_index + 1] = points[i].y;
        ret[ret_index + 2] = points[i].z;
    }
    return ret;
}
