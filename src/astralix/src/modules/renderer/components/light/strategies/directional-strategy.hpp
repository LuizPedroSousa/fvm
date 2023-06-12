#pragma once

#include "strategy.hpp"

namespace astralix {

class DirectionalStrategy : public LightStrategy {
public:
  void update(LightSource *source, Object *object,
              CameraComponent *camera) override;
};

} // namespace astralix
