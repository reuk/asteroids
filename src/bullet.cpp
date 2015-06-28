#include "bullet.h"

using namespace std;
using namespace glm;

Bullet::Bullet(GenericShader& shader_program, const Mover<vec2>& position,
               const Mover<float>& angle)
    : SpaceObject(
          shader_program,
          vector<GLfloat>{-1, -1, 0, -1, 1, 0, 1, 1, 0, 1, -1, 0},
          vector<GLfloat>{0, 0.7, 0.7, 0, 0.7, 0.7, 0, 0.7, 0.7, 0, 0.7, 0.7},
          vector<GLushort>{0, 1, 2, 3}, 0.01, position, angle) {}
