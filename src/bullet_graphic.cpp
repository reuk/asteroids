#include "bullet_graphic.h"

using namespace std;
using namespace glm;

BulletGraphic::BulletGraphic(GenericShader &shader_program)
    : StaticDrawable(shader_program,
                     vector<GLfloat>{-1, 0, 0, 0, 1, 0, 1, 0, 0, 0, -1, 0},
                     vector<GLfloat>{0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1},
                     vector<GLushort>{0, 1, 2, 3}) {
}
