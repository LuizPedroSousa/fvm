#pragma once
#include "either.hpp"

#include "exceptions/base-exception.hpp"

class Scene {
  public:
  virtual Either<BaseException, Unit> start() = 0;
  virtual void update() = 0;
};