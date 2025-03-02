#pragma once
#include "ecs/guid.hpp"
#include "strategy.hpp"

namespace astralix {

  class SpotLightStrategy : public LightStrategy {
  public:
    SpotLightStrategy();

    void update(IEntity* source, Object* object, EntityID& camera_id) override;
  };

} // namespace astralix
