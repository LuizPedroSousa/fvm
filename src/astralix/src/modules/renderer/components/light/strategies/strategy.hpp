#pragma once
#include "entities/object.hpp"

namespace astralix {

class LightStrategy {
public:
  LightStrategy() {}
  ~LightStrategy() {}
  virtual void update(Object *source, Object *object, Object *camera) = 0;
};

} // namespace astralix