#pragma once

class Updatable {
public:
  virtual ~Updatable() {}

  virtual void update() = 0;
};
