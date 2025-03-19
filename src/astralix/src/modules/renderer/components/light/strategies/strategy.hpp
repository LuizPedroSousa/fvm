#pragma once
#include "entities/ientity.hpp"
#include "entities/object.hpp"

namespace astralix {

class LightStrategy {
public:
  LightStrategy() {}
  ~LightStrategy() {}
  virtual void update(IEntity *source, Object *object, EntityID &camera,
                      size_t &index) = 0;
};

} // namespace astralix
