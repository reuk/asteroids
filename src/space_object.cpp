#include "space_object.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

SpaceObject::SpaceObject(
        Program & shader_program,
        const vector<GLfloat> & g,
        const vector<GLfloat> & c,
        const vector<GLushort> & i,
        float size,
        const Mover<vec2> & position,
        const Mover<float> & angle):
    StaticDrawable(shader_program, g, c, i),
    position(position),
    angle(angle),
    size(size)
{

}

void SpaceObject::draw() const {
    auto scale_matrix = scale(mat4(1), vec3(size));
    auto angle_matrix = rotate(mat4(1), angle.get_current(), vec3(0, 0, 1));
    auto position_matrix = translate(mat4(1), vec3(position.get_current(), 0.0f));
    auto model_matrix = position_matrix * angle_matrix * scale_matrix;

    glUniformMatrix4fv(shader_program.get_uniform_location("v_model"), 1, GL_FALSE, value_ptr(model_matrix));
    StaticDrawable::draw();
}

void SpaceObject::update() {
    position.update();
    angle.update();

    position.set_current(mod(position.get_current() + 1.0f, 2.0f) - 1.0f);
    angle.set_current(fmod(angle.get_current(), static_cast<float>(M_PI * 2)));
}
