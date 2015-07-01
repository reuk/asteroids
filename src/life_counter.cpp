#include "life_counter.h"

#define GLM_FORCE_RADIANS
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

LifeCounter::LifeCounter(StaticDrawable &ship_graphic, float size,
                         const vec2 &position)
    : graphic(&ship_graphic)
    , size(size)
    , position(position)
    , lives(0) {
}

void LifeCounter::set_lives(int l) {
    lives = l;
}

void LifeCounter::draw() const {
    auto scale_matrix = scale(mat4(1), vec3(size));

    for (auto i = 0; i != lives; ++i) {
        auto position_matrix = translate(
            mat4(1), vec3(position.x + i * 2.5 * size, position.y, 0.0f));
        auto model_matrix = position_matrix * scale_matrix;

        graphic->set_model_matrix(model_matrix);
        graphic->draw();
    }
}
