#pragma once
#include "ecs/entities/entity.hpp"
#include "imgui.h"

namespace astralix {
class Layer : public Entity<Layer> {

public:
  Layer() {}

  Layer(ENTITY_INIT_PARAMS) : ENTITY_INIT() {}

  void on_enable() override{};
  void on_disable() override{};

  virtual void start() = 0;
  virtual void update() = 0;
};
} // namespace astralix
