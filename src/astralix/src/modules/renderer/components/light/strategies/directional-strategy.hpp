#pragma once

#include "strategy.hpp"

namespace astralix {

class DirectionalStrategy : public LightStrategy {
public:
  void update(Object *source, Object *object, Object *camera) override;
};

} // namespace astralix
