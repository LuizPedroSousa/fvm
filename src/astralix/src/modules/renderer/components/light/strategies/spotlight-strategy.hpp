#pragma once
#include "guid.hpp"
#include "strategy.hpp"

namespace astralix {

class SpotLightStrategy : public LightStrategy {
public:
  SpotLightStrategy();

  void update(IEntity *source, Object *object, EntityID &camera_id,
              size_t &index) override;
};

} // namespace astralix
