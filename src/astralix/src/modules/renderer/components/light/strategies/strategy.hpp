#pragma once
#include "ecs/entities/ientity.hpp"
#include "entities/object.hpp"

namespace astralix {

class LightStrategy {
public:
  LightStrategy() {}
  ~LightStrategy() {}
  virtual void update(IEntity *source, Object *object, EntityID &camera) = 0;
};

} // namespace astralix
