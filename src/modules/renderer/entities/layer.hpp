#pragma once
#include "entities/base/entity.hpp"
#include "imgui.h"

class Layer {

  public:
  Layer() {}

  virtual void start() = 0;
  virtual void update() = 0;
};