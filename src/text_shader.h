#pragma once

#include "shader_program.h"
#include "texture.h"

#include <glm/glm.hpp>

class TextShader : public ShaderProgram {
public:
    TextShader();

    void set_model_matrix(const glm::mat4 &mat) const;
    void set_view_matrix(const glm::mat4 &mat) const;
    void set_projection_matrix(const glm::mat4 &mat) const;

private:
    void set_matrix(const std::string &s, const glm::mat4 &mat) const;

    static const std::string vertex_shader;
    static const std::string fragment_shader;
};
