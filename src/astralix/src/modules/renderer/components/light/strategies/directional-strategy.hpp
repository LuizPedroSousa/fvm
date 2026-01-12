#pragma once

#include "guid.hpp"
#include "strategy.hpp"

namespace astralix {

class DirectionalStrategy : public LightStrategy {
public:
  void update(IEntity *source, Object *object, EntityID &camera_id,
              size_t &index) override;
};

} // namespace astralix
