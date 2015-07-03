#include "ship_graphic.h"

using namespace std;
using namespace glm;

ShipGraphic::ShipGraphic(GenericShader &shader_program)
    : StaticDrawable(
          shader_program, vector<GLfloat>{0.0, 1.0, 0, 1.0, -1.0, 0, 0.5, -0.5,
                                          0, -0.5, -0.5, 0, -1.0, -1.0, 0},
          vector<GLfloat>{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
          vector<GLushort>{0, 1, 2, 3, 4}) {
}
