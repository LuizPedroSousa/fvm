#pragma once
#include "components/camera/camera-component.hpp"
#include "entities/light-source.hpp"
#include "entities/object.hpp"

namespace astralix {

class LightStrategy {
public:
  LightStrategy() {}
  ~LightStrategy() {}
  virtual void update(LightSource *source, Object *object,
                      CameraComponent *camera) = 0;
};

} // namespace astralix