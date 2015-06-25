#include "screen_boundary.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace std;
using namespace glm;

ScreenBoundary::ScreenBoundary(Program & shader_program):
    StaticDrawable(
            shader_program,
            vector<GLfloat>{
                     -1, -1,  0,
                     -1,  1,  0,
                      1,  1,  0,
                      1, -1,  0},
            vector<GLfloat>{
                      0, 1, 1,
                      0, 1, 1,
                      0, 1, 1,
                      0, 1, 1},
            vector<GLushort>{0, 1, 2, 3})
{

}

void ScreenBoundary::draw() const {
    glUniformMatrix4fv(shader_program.get_uniform_location("v_model"), 1, GL_FALSE, value_ptr(mat4(1)));
    StaticDrawable::draw();
}
