#pragma once
#include "strategy.hpp"

namespace astralix {

  class PointStrategy : public LightStrategy {
  public:
    PointStrategy() {
      //
    };
    void update(IEntity* source, Object* object, EntityID& camera_id) override;
  };

} // namespace astralix
