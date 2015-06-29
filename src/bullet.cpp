#include "bullet.h"

using namespace std;
using namespace glm;

Bullet::Bullet(StaticDrawable& bullet_graphic, const Mover<vec2>& position,
               const Mover<float>& angle)
    : SpaceObject(bullet_graphic, 0.01, position, angle) {}
