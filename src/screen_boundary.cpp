#include "screen_boundary.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace std;
using namespace glm;

ScreenBoundary::ScreenBoundary(GenericShader &shader_program)
    : StaticDrawable(shader_program,
                     vector<GLfloat>{-1, -1, 0, -1, 1, 0, 1, 1, 0, 1, -1, 0},
                     vector<GLfloat>{0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1},
                     vector<GLushort>{0, 1, 2, 3}) {
}

void ScreenBoundary::draw() const {
    shader_program.set_model_matrix(mat4(1));
    StaticDrawable::draw();
}
