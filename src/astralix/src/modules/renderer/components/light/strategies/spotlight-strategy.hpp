#pragma once
#include "strategy.hpp"

namespace astralix {

class SpotLightStrategy : public LightStrategy {
public:
  SpotLightStrategy();

  void update(Object *source, Object *object, Object *camera) override;
};

} // namespace astralix