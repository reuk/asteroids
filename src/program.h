#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "logger.h"
#include "shader.h"

#include <string>

class Program {
public:
  Program();
  virtual ~Program();

  template <GLuint I> void attach(const Shader<I> &s) const {
    glAttachShader(index, s.get_index());
  }

  template <GLuint I> void detach(const Shader<I> &s) const {
    glDetachShader(index, s.get_index());
  }

  void link() const;
  void use() const;
  static void unuse();

  GLint get_attrib_location(const std::string &name) const;
  GLint get_uniform_location(const std::string &name) const;

  bool check() const;
  bool verify() const;

  GLuint get_index() const;

private:
  GLuint index;
};
