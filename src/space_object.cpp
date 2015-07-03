#include "space_object.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

SpaceObject::SpaceObject(StaticDrawable &graphic, float size,
                         const Mover<vec2> &position, const Mover<float> &angle)
    : graphic(&graphic)
    , position(position)
    , angle(angle)
    , size(size)
    , inside(false) {
}

void SpaceObject::draw() const {
    auto scale_matrix = scale(mat4(1), vec3(size));
    auto angle_matrix = rotate(mat4(1), angle.get_current(), vec3(0, 0, 1));
    auto position_matrix =
        translate(mat4(1), vec3(position.get_current(), 0.0f));
    auto model_matrix = position_matrix * angle_matrix * scale_matrix;

    graphic->set_model_matrix(model_matrix);
    graphic->draw();
}

bool SpaceObject::inside_boundary() const {
    auto p = position.get_current();
    return all(greaterThan(p, vec2(-1.0f))) && all(greaterThan(vec2(1.0f), p));
}

bool SpaceObject::outside_boundary() const {
    return !inside_boundary();
}

void SpaceObject::update(float dt) {
    if (!inside && inside_boundary())
        inside = true;
    position.update(dt);
    if (inside)
        position.set_current(mod(position.get_current() + 1.0f, 2.0f) - 1.0f);

    angle.update(dt);
    angle.set_current(fmod(angle.get_current(), static_cast<float>(M_PI * 2)));
}

bool SpaceObject::is_hit(const SpaceObject &obj) const {
    return distance(position.get_current(), obj.position.get_current()) <
           (size + obj.size);
}
