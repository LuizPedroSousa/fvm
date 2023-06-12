#pragma once
#include "components/camera/camera-component.hpp"
#include "strategy.hpp"

namespace astralix {

class SpotLightStrategy : public LightStrategy {
public:
  SpotLightStrategy();

  void update(LightSource *source, Object *object,
              CameraComponent *camera) override;
};

} // namespace astralix