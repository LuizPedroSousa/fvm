#pragma once
#include "strategy.hpp"

namespace astralix {

class PointStrategy : public LightStrategy {
public:
  PointStrategy(){
      //
  };
  void update(Object *source, Object *object, Object *camera) override;
};

} // namespace astralix