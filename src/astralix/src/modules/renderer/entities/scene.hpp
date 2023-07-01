#pragma once
#include "either.hpp"

#include "exceptions/base-exception.hpp"

namespace astralix {

class Scene {
public:
  Scene(){};

  virtual void start() = 0;
  virtual void update() = 0;

  ~Scene() {}
};

} // namespace astralix