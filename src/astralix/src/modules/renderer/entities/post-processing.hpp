#pragma once

#include "ecs/entities/entity.hpp"

namespace astralix {
class PostProcessing : public Entity<PostProcessing> {
public:
  PostProcessing(ENTITY_INIT_PARAMS);

  void on_enable() override{};
  void on_disable() override{};
};
} // namespace astralix