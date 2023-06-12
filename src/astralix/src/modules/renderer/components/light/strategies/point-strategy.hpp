#pragma once
#include "strategy.hpp"

namespace astralix {

class PointStrategy : public LightStrategy {
public:
  PointStrategy(){
      //
  };
  void update(LightSource *source, Object *object,
              CameraComponent *camera) override;
};

} // namespace astralix