#pragma once

#include "ecs/entities/entity.hpp"

namespace astralix {
class Skybox : public Entity<Skybox> {
public:
  Skybox(ENTITY_INIT_PARAMS, const ResourceID cubemap_id,
         const ResourceID skybox_id);


  void start();
  void update();

  void on_enable() override{};
  void on_disable() override{};
};
} // namespace astralix