#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

std::vector<GLfloat> format(const std::vector<glm::vec3> &points);
std::vector<GLushort> indices(int points);
